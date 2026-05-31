#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdio.h>      //Потоки ввода-вывода
#include <stdlib.h>     //exit()
#include <string.h>     //Работа со строками и памятью (memset())
#include <unistd.h>     // close()
#include <arpa/inet.h>  //Работа с IPv4 адресами
#include <netinet/ip.h> //IP заголовок
#include <netinet/udp.h>//UDP заголовок
#include <signal.h>     //Для сигнала прерывания
//#include <pthread.h>    //Для потоков
#include <sys/wait.h>

//#define CLIENT_PORT 5042    //Порт клиента
#define SERVER_PORT 5000    //Порт сервера
#define BUF_SIZE 2048      //Размер буфера raw пакетов

#endif /* SETTINGS_H_ */