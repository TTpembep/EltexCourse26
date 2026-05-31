#include "chat.h"

static int stop = 0;
void listener(int sig) { stop = 1; }

int same_client(struct sockaddr_in *a, struct sockaddr_in *b) { //Функция сравнения двух адресов
    return a->sin_addr.s_addr == b->sin_addr.s_addr &&      //Если ip-адреса равны
           a->sin_port == b->sin_port;                      //А также порты равны - возвращает 1
}

int main() {
    int sockfd; /* Дескриптор сокета */
    int clilen, n; /* Переменные для различных длин и количества символов */
    char buf[BUF_SIZE]; /* Массив для принятой и отсылаемой строки */
    char outputMSG[BUF_SIZE];
    char serviceMsg1[] = "SERVER:Waiting for second client.\n";
    char serviceMsg2[] = "SERVER:Chatroom is full.\n";
    char serviceMsg3[] = "SERVER:User disconnected.\n";
    struct sockaddr_in servaddr, cliaddr; /* Структуры для адресов сервера и клиента */
    
    struct sockaddr_in clients[2];  //Массив подключённых клиентов
    int connectCounter = 0;  //Счётчик подключённых клиентов

    /* Заполняем структуру для адреса сервера */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    /* Создаем UDP сокет */
    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror(NULL); /* Печатаем сообщение об ошибке */
        exit(1);
    }
    /* Настраиваем адрес сокета */
    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror(NULL);
        close(sockfd);
    exit(1);
    }
    printf(">Server started. Port:%d\n", PORT); //Логирование в консоль сервера

    while(!stop) {
        /* Основной цикл обслуживания*/
        clilen = sizeof(cliaddr);
        /* Ожидаем прихода запроса от клиента и читаем его */
        if((n = recvfrom(sockfd, buf, BUF_SIZE - 1, 0, (struct sockaddr *)&cliaddr, &clilen)) < 0){
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        buf[n] = '\0';  //Добавление на конец символа конца строки, чтобы не передавать выводились мусорные значения


        int currentClientID = -1; //Идентификатор текущего клиента в массиве подключённых клиентов
        for (int i = 0; i < connectCounter; i++) {
            if (same_client(&clients[i], &cliaddr)) {   //Проверка есть ли клиент, от которого было получено сообщение, в массиве подключённых
                currentClientID = i;    //Если да - то записываем его ID, чтобы затем переслать сообщение другому подключенному клиенту 
                break;
            }
        }
        if (currentClientID != -1 && strcmp(buf, "shutdown\n\0") == 0 ) {   //Обработка отключения клиента от чата
            printf("Client %s:%d disconnected.\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));   //Логирование в консоль сервера
            int temp;
            if (currentClientID == 1){
                memset(&clients[1], 0, sizeof(clients[1]));
                connectCounter--;
            }else if(currentClientID == 0){
                clients[0] = clients[1];
                memset(&clients[1], 0, sizeof(clients[1]));
                connectCounter--;
            }
            else {printf("Something went wrong while disconnecting\n");}    //Логирование в консоль сервера
            if (connectCounter > 0) {   //Отправка другому клиента сообщения что его собеседник отключился
                if(sendto(sockfd, serviceMsg3, strlen(serviceMsg3) + 1, 0, (struct sockaddr *) &clients[0], sizeof(clients[0])) < 0){
                perror(NULL);
                close(sockfd);
                exit(1);
                }
            }
        }
        if (currentClientID == -1 && connectCounter < 2) {  //Если клиент не записан в массиве подключенных и всего подключённых пользователей меньше 2
            clients[connectCounter] = cliaddr;  //Записываем клиента в список подключённых
            currentClientID = connectCounter;   //
            connectCounter++;
            printf("Client %d connected: %s:%d\n", connectCounter, inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));   //Логирование в консоль сервера
            if (connectCounter == 2) { printf("Chat is ready\n"); } //Логирование в консоль сервера
            //continue;
        }else if (currentClientID == -1 && connectCounter > 1) {    //Если пытается подключиться 3 и более клиент - вывод сообщения что чат заполнен
            if(sendto(sockfd, serviceMsg2, strlen(serviceMsg2) + 1, 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0){
            perror(NULL);
            close(sockfd);
            exit(1);
            }
            printf("%s", serviceMsg2);  //Логирование в консоль сервера
            continue;
        }
        if (connectCounter < 2){    //Вывод сообщения что ожидается второй клиент
            if(sendto(sockfd, serviceMsg1, strlen(serviceMsg1) + 1, 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0){
            perror(NULL);
            close(sockfd);
            exit(1);
            }
            printf("%s", serviceMsg1);  //Логирование в консоль сервера
            continue;
        }

        int other;  //Определение какому клиенту пересылать сообщение
        if (currentClientID == 0) {
            other = 1;
        }else { other = 0; }

        snprintf(outputMSG, sizeof(outputMSG), "User%d:%s", ntohs(cliaddr.sin_port), buf);  //Форматирование сообщения
        /* Принятый текст отправляем по адресу другого клиента */
        if(sendto(sockfd, outputMSG, strlen(outputMSG) + 1, 0, (struct sockaddr *) &clients[other], sizeof(clients[other])) < 0){
            perror(NULL);
            close(sockfd);
            exit(1);
        } /* Уходим ожидать новую датаграмму*/
        printf("[%s:%d] %s", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), buf);    //Логирование в консоль сервера
    }
    close(sockfd);  /* По окончании работы закрываем дескриптор сокета */
    return 0;
}