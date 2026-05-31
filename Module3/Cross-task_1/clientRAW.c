#include "settings.h"

//static int stop = 0;
int sockfd; //Дескриптор сокета
struct sockaddr_in servaddr;  //Структура для адреса сервера
char exitMsg[] = "shutdown";    //Служебное слово для отправки на сервер
pid_t pid;  //Идентификатор дочернего процесса
uint16_t CLIENT_PORT;   //Порт клиента

void udpMsgSender(char *buf){   //Формирование UDP-пакета и отправка через RAW-сокет
    char packet[BUF_SIZE];
    struct udphdr *udp = (struct udphdr *)packet;
    char *payload = packet + sizeof(struct udphdr);
    //Заполнение UDP заголовка
    udp->source = htons(CLIENT_PORT);
    udp->dest = htons(SERVER_PORT);
    udp->len = htons(sizeof(struct udphdr) + strlen(buf));
    udp->check = 0;     //Игнорируется в Linux
    //Копирование полезной нагрузки
    memcpy(payload, buf, strlen(buf));
    //Отправка UDP пакета
    sendto(sockfd, packet, sizeof(struct udphdr) + strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
}

//Я понимаю что такие потенциально долгие и тяжелые операции
//нельзя писать в обработчик прерывания, но что поделать. 
//Такая вот не самая лучшая реализация получилась..
void listener(int sig) {    //Обработчик прерывания
    kill(pid, SIGINT);  //Сигнал прерывания для дочернего процесса
    wait(NULL); //Ожидание заверщения дочернего процесса
    if (pid != 0) { udpMsgSender(exitMsg); }//Отправка сообщения серверу
    close(sockfd);  //Закрытие сокета
    exit(0);    //Завершение работы
    }

int main(int argc, char *argv[]) {
    if(argc != 3){  //Проверка аргументов запуска
        printf("Usage: ./client <IP address> <Port>\n");
        exit(1);
    }
    signal(SIGINT, listener);   //Обработка сигнала прерывания ^C
    char buf[BUF_SIZE];  //Переменная для сообщения
    CLIENT_PORT = atoi(argv[2]);    //Порт указывается как аргумент запуска, чтобы можно было запустить несколько клиентов одновременно
    
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);    //Создание RAW UDP сокета
    if(sockfd < 0) {
        perror("socket");
        return 1;
    }

    //Заполнение структуры для адреса сервера
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);

    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {    //Попытка перевести аргумент запуска в IP-адрес
        printf("Invalid server IP address\n");
        close(sockfd);
        return 1;
    }
    printf("Connected to server %s:%d\n", argv[1], SERVER_PORT);

    pid = fork(); //Создание дочернего процесса для аснихронного взаимодействия
    if (pid < 0){
        perror("fork");
        return 1;
    }
    if (pid == 0){  //Дочерний процесс обрабатывает входящие сообщения
        while (1) { //Обработка UDP-пакета, полученного через RAW-сокет
            struct sockaddr_in from;
            socklen_t fromlen = sizeof(from);
            int n = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen);
            if (n < 0) {
                perror("recvfrom");
                continue;
            }
            struct iphdr *ip = (struct iphdr *)buf;
            if(ip->protocol != IPPROTO_UDP) { continue; }   //Если протокол не UDP - отбрасываем пакет
            struct udphdr *udp = (struct udphdr *) (buf + ip->ihl * 4);
            if(ntohs(udp->dest) != CLIENT_PORT) { continue; }   //Если указан не порт клиент - отбрасываем пакет
            char *payload = buf + ip->ihl * 4 + sizeof(struct udphdr);
            int payload_len = ntohs(udp->len) - sizeof(struct udphdr);
            if(payload_len <= 0) { continue; }

            char msg[1024];
            memcpy(msg, payload, payload_len);
            msg[payload_len] = '\0';
            printf("%s>", msg);
            fflush(stdout);
        }
    }
    else{   //Родительский процесс отвечает за отправку сообщений
        while (1){
            printf(">");
            if (fgets(buf, sizeof(buf), stdin) == NULL) { break; }
            udpMsgSender(buf);
            if (strcmp(buf, "shutdown\n") == 0) { 
                break;
            }
        }
    }
    kill(pid, SIGINT);  //Сигнал прерывания для дочернего процесса
    wait(NULL); //Ожидание заверщения дочернего процесса
    udpMsgSender(exitMsg); //Отправка сообщения серверу
    close(sockfd);  //Закрытие сокета
    exit (0);
}