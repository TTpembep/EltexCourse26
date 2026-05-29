#include "coolq.h"

coolQ* createNode(const char *text, int priority){
    coolQ *node = (coolQ*)malloc(sizeof(coolQ));
    if(node == NULL){
        printf("Memory allocation error.\n");
        return NULL;
    }
    strcpy(node->data, text);
    node->priority = priority;
    node->prev = NULL;
    node->next = NULL;
    return node;
}


void push(coolQ **head, const char *text, int priority){
    coolQ *newNode = createNode(text, priority);
    if(newNode == NULL){
        printf("Memory allocation error.\n");
        return;
    }
    if(*head == NULL){  //Пустая очередь
        *head = newNode;
        return;
    }
    coolQ *temp = *head;
    while(temp != NULL && temp->priority <= priority){  //Сортировка по приоритету
        temp = temp->next;
    }
    if(temp == NULL){    //Вставка в конец очереди
        coolQ *last = *head;
        while(last->next != NULL){
            last = last->next;
        }
        last->next = newNode;
        newNode->prev = last;
        return;
    }
    if(temp == *head){  //Вставка в начало
        newNode->next = *head;
        (*head)->prev = newNode;
        *head = newNode;
        return;
    }
    coolQ *previous = temp->prev;   //Вставка в середину
    previous->next = newNode;
    newNode->prev = previous;
    newNode->next = temp;
    temp->prev = newNode;
    return;
}


void pop(coolQ **head){
    if(*head == NULL){
        printf("Queue is empty.\n");
        return;
    }
    coolQ *temp = *head;
    *head = (*head)->next;
    if(*head != NULL){
        (*head)->prev = NULL;
    }
    free(temp);
    return;
}

coolQ* extractByPriority(coolQ **head, int priority){
    if(*head == NULL){
        return NULL;
    }
    coolQ *temp = *head;
    while(temp != NULL){
        if(temp->priority == priority){
            if(temp == *head){
                *head = temp->next;
                if(*head != NULL){ (*head)->prev = NULL; }
            } else {
                temp->prev->next = temp->next;
                if(temp->next != NULL) { temp->next->prev = temp->prev; }
            }
            temp->next = NULL;
            temp->prev = NULL;
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

coolQ* extractNotLower(coolQ **head, int priority){
    if(*head == NULL){ return NULL; }
    coolQ *temp = *head;
    while(temp != NULL){
        if(temp->priority <= priority){
            if(temp == *head){
                *head = temp->next;
                if(*head != NULL){
                    (*head)->prev = NULL;
                }
            } else {
                temp->prev->next = temp->next;
                if(temp->next != NULL) { temp->next->prev = temp->prev; }
            }
            temp->next = NULL;
            temp->prev = NULL;
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void printQueue(coolQ *head){
    if(head == NULL){
        printf("Queue is empty.\n");
        return;
    }
    coolQ *temp = head;
    while(temp != NULL){
        printf("Data: %-20s | Priority: %3d\n", temp->data, temp->priority);
        temp = temp->next;
    }
    return;
}


void freeQueue(coolQ **head) {
    while(*head != NULL){
        pop(head);
    }
    return;
}


void menuPush(coolQ **queue) {
    char text[30];
    int priority;
    printf("Enter message: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';   //Удаление символа переноса строки
    printf("Enter priority [0 - 255]: ");
    while (1){
        if (scanf("%d", &priority) != 1){  //Обработка ввода символов, отличных от чисел
            printf("Not a number, try again: ");
            while(getchar()!='\n'); //Очистка буфера
        }
        else if (priority < 0 || priority > 256){
            printf("Out of range, try again: ");
        }else {break;}
    }
    while(getchar() != '\n');

    push(queue, text, priority);
    printf("Element added successfully!\n");
    return;
}