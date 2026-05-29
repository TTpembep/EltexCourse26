#ifndef COOLQ_H_
#define COOLQ_H_

#include <stdio.h>  //Для работы с потокам ввода/вывода
#include <stdlib.h> //Для работы с динамической памятью
#include <string.h>

typedef struct coolQ
{
    char data[30];
    int priority;

    struct coolQ *prev;
    struct coolQ *next;

} coolQ;

coolQ* createNode(const char *text, int priority);
void push(coolQ **head, const char *text, int priority);
void pop(coolQ **head);
coolQ* extractByPriority(coolQ **head, int priority);
coolQ* extractNotLower(coolQ **head, int priority);
void printQueue(coolQ *head);
void freeQueue(coolQ **head);
void menuPush(coolQ **queue);

#endif /* COOLQ_H_ */