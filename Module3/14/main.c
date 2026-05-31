#include <stdio.h>  //Потоки ввода-вывода
#include <stdlib.h> //exit()
#include <unistd.h> //close()
#include <arpa/inet.h>  //Функции по типу inet_ntoa()
#include <netinet/ip.h> //struct iphdr
#include <netinet/udp.h>    //struct udphdr
#include <sys/socket.h> //Для сокетов

#define PORT 51000  //Порт для прослушивания

int main(){
    int sockfd; //Дескриптор RAW-сокета
    char buffer[65536]; //Буфер для хранения перехваченного IP-пакета
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);    //Создание RAW-сокета
    if (sockfd < 0){
        perror("socket");
        exit(1);
    }
    while (1){
        int n = recv(sockfd, buffer, sizeof(buffer), 0);    //Ожидание пакета
        if (n < 0) { continue; }

        struct iphdr *ip = (struct iphdr *)buffer;  //Разбор IP-заголовка
        int iphdrlen = ip->ihl * 4; //Перевод длины IP-заголовка из 32-битных слов в байты
        struct udphdr *udp = (struct udphdr *)(buffer + iphdrlen);

        int src = ntohs(udp->source);   //Получение номеров портов
        int dst = ntohs(udp->dest);

        if (src != PORT && dst != PORT) { continue; }   //Пропуск пакетов с другими портами

        FILE *dump = fopen("dump.bin", "ab");   //Формирование бинарного дампа данных
        fwrite(&n, sizeof(n), 1, dump);
        fwrite(buffer, 1, n, dump);
        fclose(dump);

        char *payload = buffer + iphdrlen + sizeof(struct udphdr);  //Указатель на полезные данные
        int payload_len = n - iphdrlen - sizeof(struct udphdr); //Длина полезных данных
        printf("[%d -> %d] ", src, dst);    //Вывод в терминал
        printf("%.*s\n", payload_len, payload);
    }
}