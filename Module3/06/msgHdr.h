#ifndef MSGHDR_H_
#define MSGHDR_H_

#include <stdio.h>      //Для потоков ввода-вывода
#include <sys/types.h>  //Нужен для типа данных key_t
#include <sys/ipc.h>    //Для IPC (Межпроцессное взаимодействие)
#include <sys/msg.h>    //Для System V (Структуры очередей сообщений)
#include <string.h>
#include <stdlib.h>     //for argc argv
#include <unistd.h>

#define MSGSZ 64        //Определение размера сообщения
#define SERVER_TYPE 10  //ID сервера
#define MAX_CLIENTS 8
#define QUEUE_KEY 42

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;


#endif /* MSGGDR_H_ */