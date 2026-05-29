#include "coolq.h"
#include <time.h>

void generateMessages(coolQ **queue, int count){
    char text[30];
    int priority;
    for(int i = 0; i < count; i++){
        sprintf(text, "Message_%d", i + 1);
        priority = rand() % 256;
        push(queue, text, priority);
    }
    printf("%d messages generated successfully.\n", count);
}

void testExtraction(coolQ **queue){
    coolQ *item;
    printf("\tTest begins.\n");
    printQueue(*queue);
    printf("Pop test\n");
    pop(queue);
    printQueue(*queue);
    printf("Priority extract test\n");
    int priority = rand() % 256;
    printf("Searching priority: %d\n", priority);
    item = extractByPriority(queue, priority);
    if(item != NULL){
        printf("Extracted: %s | priority: %d\n", item->data, item->priority);
        free(item);
    }
    else{
        printf("Element not found.\n");
    }
    printQueue(*queue);
    printf("Not lower priority test\n");
    priority = rand() % 256;
    printf("Required priority <= %d\n", priority);
    item = extractNotLower(queue, priority);
    if(item != NULL){
        printf("Extracted: %s | priority: %d\n", item->data, item->priority);
        free(item);
    } else {
        printf("No suitable element found.\n");
    }
    printQueue(*queue);
}


int main(){
    srand(time(NULL));
    coolQ *queue = NULL;
    int option = -1;
    int count;

    while(option != 0){
        printf("## The Cool Queue ##\n");
        printf("1. Add element manually\n");
        printf("2. Extract first\n");
        printf("3. Print queue\n");
        printf("4. Extract by priority\n");
        printf("5. Extract not lower\n");
        printf("6. Generate random messages\n");
        printf("7. Automatic test\n");
        printf("0. Exit\n");
        printf("Enter option: ");
        scanf("%d", &option);
        while(getchar() != '\n');

        coolQ *item = NULL;
        int priority;
        switch(option){
            case 1:
                menuPush(&queue);
                break;

            case 2:
                pop(&queue);
                break;

            case 3:
                printQueue(queue);
                break;

            case 4:
                printf("Enter priority: ");
                scanf("%d", &priority);
                while(getchar() != '\n');
                item = extractByPriority(&queue, priority);
                if(item != NULL){
                    printf("Extracted: %s | priority: %d\n", item->data, item->priority);
                    free(item);
                } else {
                    printf("Element not found.\n");
                }
                break;

            case 5:
                printf("Enter maximum priority value: ");
                scanf("%d", &priority);
                while(getchar() != '\n');
                item = extractNotLower(&queue, priority);
                if(item != NULL){
                    printf("Extracted: %s | priority: %d\n", item->data, item->priority);
                    free(item);
                } else {
                    printf("No suitable element found.\n");
                }
                break;
            case 6:
                printf("How many messages generate: ");
                scanf("%d", &count);
                while(getchar() != '\n');
                generateMessages(&queue, count);
                break;
            case 7:
                testExtraction(&queue);
                break;

            case 0:
                printf("Program finished.\n");
                break;

            default:
                printf("Wrong option.\n");
                break;
        }
    }
    freeQueue(&queue);
    return 0;
}