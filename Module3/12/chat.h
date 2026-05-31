#ifndef CHAT_H_
#define CHAT_H_

#include <sys/types.h>  //Базовые системные типы (pid_t, size_t)
#include <sys/socket.h> //Основные функции сокетов и константы
#include <netinet/in.h> //Структуры и константы IPv4
#include <arpa/inet.h>  //Функции для работы с IP-адресами (inet_ntoa)
#include <string.h> //Работа со строками и памятью
#include <stdio.h>  //Для потоков ввода-вывода
#include <unistd.h> //Функции по типу close(sockfd)
#include <stdlib.h> //Общие функции exit()
#include <signal.h> //Для сигнала прерывания
#include <sys/wait.h>   //Для функции wait()

#define PORT 51000      //Порт сервера
#define BUF_SIZE 1000   //Размер буфера

#endif /* CHAT_H_ */