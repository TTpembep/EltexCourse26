#include "settings.h"

typedef struct Client {     //Односвязный список для информации о клиентах
    uint32_t ip;        //IP адрес клиента
    uint16_t port;      //UDP порт клиента
    int counter;  //Счётчик сообщений полученных от клиента
    struct Client *next;    //Указатель на следующий узел
} Client;

Client *clients = NULL; //Указатель на начало списка клиентов

Client *find_client(uint32_t ip, uint16_t port){    //Функция поиска клиента с заданным IP-адресом и портом
    Client *current = clients;  //Указатель для обхода списка
    while(current){
        if(current->ip == ip && current->port == port) {
            return current; //Возвращаем текущего клиента если значения совпали
        }
        current = current->next;    //Проход по всему списку
    }
    return NULL;    //Если клиент не найден
}

Client *add_client(uint32_t ip, uint16_t port){ //Функция добавления нового клиента в начало списка
    Client *client = (Client*)malloc(sizeof(Client));   //Выделение памяти
    if(client == NULL){
        perror("malloc");
        exit(1);
    }
    client->ip = ip;    //Задание IP-адреса
    client->port = port;    //Задание порта
    client->counter = 0;    //Задание счётчика
    client->next = clients; //Встака нового узла в начало списка
    clients = client;   //Назначение узла новым началом списка
    return client;  //Возвращение указателя на структуру
}

void remove_client(uint32_t ip, uint16_t port){ //Функция удаления клиента из списка
    Client *current = clients;  //Указатель на текущий элемент
    Client *previous = NULL;    //Указатель на предыдущий элемент
    while(current){
        if(current->ip == ip && current->port == port) {    //Если найден нужный клиент
            //Если удаляемый элемент не первый
            if(previous) {  //Случай когда элемент не первый
                previous->next = current->next; }   //Сохранение связности списка
            else {  //Случай когда элемент первый
                clients = current->next;    //Переназначение указателя на список
            }
            free(current);  //Освобождение памяти
            return;
        }
        previous = current; //Обход списка
        current = current->next;
    }
}

int main() {
    int rawSock;    //Дескриптор RAW-сокета
    char buffer[BUF_SIZE];  //Переменная для сообщения

    rawSock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);   //Создание RAW-сокета
    if(rawSock < 0) {
        perror("socket");
        return 1;
    }
    printf("Server started. Port:%d\n", SERVER_PORT);

    while(1){
        struct sockaddr_in senderAddr;  //Структура адреса отправителя
        socklen_t addrLen = sizeof(senderAddr);
        
        int bytes = recvfrom(rawSock, buffer, sizeof(buffer), 0, (struct sockaddr*)&senderAddr, &addrLen);  //Приём IP пакета
        if(bytes <= 0) { continue; }
        
        struct iphdr *ipHeader = (struct iphdr*)buffer; //Указатель на IP заголовок
        
        if(ipHeader->protocol != IPPROTO_UDP) { continue; } //Если пакет не UDP - отбрасываем
       
        struct udphdr *udpHeader = (struct udphdr*) (buffer + ipHeader->ihl * 4);   //Указатель на UDP заголовок
        if(ntohs(udpHeader->dest) != SERVER_PORT) { continue; } //Если указан порт не сервера - отбрасываем
        uint32_t clientIP = ipHeader->saddr;    //IP адрес клиента
        uint16_t clientPort = ntohs(udpHeader->source); //Порт клиента
        
        char *payload = buffer + ipHeader->ihl * 4 + sizeof(struct udphdr); //Указатель на данные UDP пакета
        int payloadLen = ntohs(udpHeader->len) - sizeof(struct udphdr); //Длина полезных данных
        if(payloadLen <= 0) { continue; }
        
        char msg[BUF_SIZE]; //Буфер для текстового сообщения
        memcpy(msg, payload, payloadLen);   //Копирование полезной нагрузки
        msg[payloadLen] = '\0';

        printf("[%s:%d] %s\n", inet_ntoa(*(struct in_addr*) &clientIP), clientPort, msg);   //Логирование в консоль

        if(strcmp(msg, "shutdown") == 0) {  //Обработка служебного слова для завершения соединения
            remove_client(clientIP, clientPort);
            printf("Client %s:%d disconnected\n", inet_ntoa( *(struct in_addr*) &clientIP), clientPort);    //Логирование в консоль
            continue;
        }
        Client *client = find_client(clientIP, clientPort); //Поиск клиента в списке
        if(client == NULL) {    //Добавление нового клиента в список
            client = add_client(clientIP, clientPort);
            printf("New client connected: %s:%d\n", inet_ntoa(*(struct in_addr*) &clientIP), clientPort);   //Логирование в консоль
        }
        client->counter++;  //Увеличение счётчика сообщений от клиента


        msg[strlen(msg) - 1] = '\0';    //Чтобы клиент получал сообщение без лишнего переноса строки
        char response[BUF_SIZE];    //Буфер ответа клиенту
        snprintf(response, sizeof(response), "SERVER:%s\t%d\n", msg, client->counter); //Формирование ответа клиенту
        
        char packet[BUF_SIZE];  //Формирование UDP-пакета и отправка через RAW-сокет
        struct udphdr *udp = (struct udphdr *)packet;
        payload = packet + sizeof(struct udphdr);
            //Заполнение UDP заголовка
        udp->source = htons(SERVER_PORT);
        udp->dest = htons(clientPort);
        udp->len = htons(sizeof(struct udphdr) + strlen(response));
        udp->check = 0;
            //Полезная нагрузка
        memcpy(payload, response, strlen(response));
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family =   AF_INET;
        addr.sin_addr.s_addr = clientIP;
        sendto(rawSock, packet, sizeof(struct udphdr) + strlen(response), 0, (struct sockaddr *)&addr, sizeof(addr));
        
        printf("Reply: %s\n", response);    //Логирование в консоль
    }
    close(rawSock); //Закрытие сокета
    return 0;
}