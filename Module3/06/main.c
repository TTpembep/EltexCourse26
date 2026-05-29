#include "msgHdr.h"

int main(void){
    int msqid;  //Дескриптор очереди. С его помощью происходит взаимдействие с очередью
    int msgflg = IPC_CREAT | 0666;  //Флаги, которые указываются при создании очереди (напр. права доступа к очереди)
    key_t key = QUEUE_KEY;      //Ключ (имя), который присваивается очереди при её создании

    message_buf sbuf;   //Структура которая хранит сообщение и его тип  (Для отправки)
    message_buf rbuf;   //И для получения сообщения

    int clients[MAX_CLIENTS];   //Массив подключенных пользователей
    int clientCount = 0;    //Количество подключенных пользователей

    printf(">Starting server with key %d and flag %#o\n",key,msgflg);
    if ((msqid = msgget(key, msgflg )) < 0) {   //Создание очереди сообщений
        perror("msgget");
        return 1;
    }else { printf(">msgget success msqid = %d\n", msqid);}

    while(1){
        if (msgrcv(msqid, &rbuf, MSGSZ, SERVER_TYPE, 0) < 0) {  //Ожидание сообщения
            perror("msgrcv");
            return 1;
        }
        int clientId;   //Переменная для записи id клиента
        char message[MSGSZ];    //Переменная для сообщения

        sscanf(rbuf.mtext, "%d:%63[^\n]", &clientId, message);  //Отделение id от сообщения
        printf("Client %d: %s\n", clientId, message);   //Логирование в консоль

        int exists = 0; 
        for (int i = 0; i < clientCount; i++) { //Проверка есть ли клиент в списке подключенных клиентов
            if (clients[i] == clientId) {
                exists = 1;
                break;
            }
        }
        
        if (!exists) {  //Если клиент новый - добавляем в таблицу подключённых
            clients[clientCount++] = clientId;
            printf("Client %d connected\n", clientId);
            if (strcmp(message, "#connect") == 0) { continue; } //Обработка служебного сообщения, чтобы его не пересылать
        }

        if (strcmp(message, "shutdown") == 0) { //Обработка служебного слова для отключения пользователя
            printf("Client %d disconnected\n", clientId);
            for (int i = 0; i < clientCount; i++) {
                if (clients[i] == clientId) {
                    for (int j = i; j < clientCount - 1; j++) {
                        clients[j] = clients[j + 1];    //Удаление клиента из массива подключённых пользователей
                    }
                    clientCount--;  //Уменьшение счётчика подключенных клиентов
                    break;
                }
            }
            continue;   //Сообщение shutdown не будет переслано остальным клиентам
        }
        for (int i = 0; i < clientCount; i++) { //Рассылка сообщения подключённым пользователям
            if (clients[i] == clientId) {continue;} //Пропуск отправителя сообщения
            sbuf.mtype = clients[i];    //Адресация сообщения клиенту
            snprintf(sbuf.mtext, MSGSZ, "FROM %d: %.40s", clientId, message);   //Формирование сообщения
            if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {  //Отправка сообщения
                perror("msgsnd");
            }
        }   
    }
    msgctl(msqid, IPC_RMID, NULL);  //Удаление очереди сообщений, но цикл сюда никогда не дойдёт.
    printf("Message queue removed\n");
    return 0;
}