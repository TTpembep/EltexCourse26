#include "msgHdr.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {    //При запуске должно быть только имя исполняемого файла и id
        printf("Choose user ID: ./client 20\n");
        return 1;
    }
    if (atoi(argv[1])%10 != 0 || atoi(argv[1]) == SERVER_TYPE){ //Проверка что id соответсвует правилам задания
        printf("Allowed IDs: 20, 30, 40..\n");                  //И не является id сервера
        return 1;
    }
    int clientId = atoi(argv[1]);   //Идентификатор клиента
    int msqid;              //Дискриптор очереди
    key_t key = QUEUE_KEY;  //Ключ очереди

    message_buf sbuf;   //Структура для отправки сообщения
    message_buf rbuf;   //Структура для принятия сообщения

    if ((msqid = msgget(key, 0666)) < 0) {  //Получение дискриптора очереди, если она существует
        perror("msgget");
        return 1;
    }
    sbuf.mtype = SERVER_TYPE;   //Указание типа сообщения, который принимает сервер
    snprintf(sbuf.mtext, MSGSZ, "%d:#connect", clientId);   //Отправка служебного сообщения: Псевдо "авторизация" на сервере
    msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0);        //Сервер добавит пользователя в список подключенных и будет пересылать ему сообщения
    printf("Connected as client %d\n", clientId);           //Даже если пользователь ещё сам не отправил сообщение

    while (1) {
        printf("Enter message: ");
        char message[MSGSZ];
        fgets(message, MSGSZ, stdin);   //Запись из потока ввода в переменную
        message[strcspn(message, "\n")] = 0;    //Удаление символа \n из массива символов

        if (strlen(message) == 0){  //Если поле сообщения пустое, пользователь просто нажал enter
            while (1) {             //Будут получены сообщения из очереди
                if (msgrcv(msqid, &rbuf, MSGSZ, clientId, IPC_NOWAIT) < 0) {
                    break;
                } printf("%s\n", rbuf.mtext);
            } continue;             //Без отправки нового сообщения
        }

        snprintf(sbuf.mtext, MSGSZ, "%d:%.40s", clientId, message); //Добавление id пользователя в сообщение
        if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {  //Отправка сообщения в очередь
            perror("msgsnd");
            return 1;
        }

        if (strcmp(message, "shutdown") == 0) {    //Обработка служебного слова для завершения соединения
            break;                                 //Также будет завершена работа программы
        }
        
        while (1) { //Получение всех сообщений из канала, с типом msgbuf.mtyp = clientId
            if (msgrcv(msqid, &rbuf, MSGSZ, clientId, IPC_NOWAIT) < 0) {
                break;
            }
            printf("%s\n", rbuf.mtext); //Вывод сообщений в консоль
        }
    }
    return 0;
}