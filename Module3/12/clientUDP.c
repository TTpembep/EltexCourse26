#include "chat.h"

static int stop = 0;
void listener(int sig) { stop = 1; }

int main(int argc, char **argv) {
    /* Сначала проверяем наличие второго аргумента в
    командной строке. При его отсутствии ругаемся и прекращаем работу */
    if(argc != 2){
        printf("Usage: ./client <IP address>\n");
        exit(1);
    }
    int sockfd; /* Дескриптор сокета */
    struct sockaddr_in servaddr; /* Структура для адреса сервера */
    char buf[BUF_SIZE]; //Переменная для сообщения
    
    /* Создаем UDP сокет */
    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror(NULL); /* Печатаем сообщение об ошибке */
        exit(1);
    }

    /*  Ядро Linux автоматически выполняет неявную привязку сокета
    // Заполняем структуру для адреса клиента   
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // Настраиваем адрес сокета 
    if(bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0){
        perror(NULL);
        close(sockfd); // По окончании работы закрываем дескриптор сокета 
        exit(1);}
    */

    /* Заполняем структуру для адреса сервера */
    bzero(&servaddr, sizeof(servaddr)); //Зануление структуры
    servaddr.sin_family = AF_INET;      //IPv4 протокол интернет
    servaddr.sin_port = htons(PORT);    //Указание порта
    if(inet_aton(argv[1], &servaddr.sin_addr) == 0) {  //Используем аргумент запуска как IP-адрес
        printf("Invalid IP address\n");
        close(sockfd); /* По окончании работы закрываем дескриптор сокета */
        exit(1);
    }
    
    strcpy(buf, "CONNECTED\n"); //Отправляем серверу сообщение, чтобы стать пользователем подключённым к чату 
    /* Отсылаем датаграмму */
    if(sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    pid_t pid = fork(); //Создание дочернего процесса для аснихронного взаимодействия
    if (pid < 0){
        perror("fork");
        return 1;
    }
    if (pid == 0){  //Дочерний процесс обрабатывает входящие сообщения
        while (!stop) {
            int n = recvfrom(sockfd, buf, BUF_SIZE - 1, 0, NULL, NULL);
            if (n < 0) {
                perror("recvfrom");
                continue;
            }
            buf[n] = '\0';
            printf("%s>", buf);
            fflush(stdout);
        }
    }
    else{   //Родительский процесс отвечает за отправку сообщений
        while (!stop){
            printf(">");
            if (fgets(buf, sizeof(buf), stdin) == NULL) { break; }
            sendto(sockfd, buf,strlen(buf) + 1, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
            if (strcmp(buf, "shutdown\n") == 0) {
                stop = 1; 
                break;
                }
        }
    }
    kill(pid, SIGINT);  //Сигнал прерывания для дочернего процесса
    wait(NULL); //Ожидание заверщения дочернего процесса
    close(sockfd);  /* По окончании работы закрываем дескриптор сокета */
    return 0;
}