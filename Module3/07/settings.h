#ifndef SETTINGS_H_
#define SETTING_H_

#include <stdio.h>  //Для потоков ввода-вывода
#include <string.h> //Для работы со строками
#include <mqueue.h> //Для Очереди сообщений POSIX
#include <fcntl.h>  //Для определения констант при открытии очереди

#define Q1 "/queue1"    //Очередь клиент1 -> клиент2
#define Q2 "/queue2"    //Очередь клиент2 -> клиент1

#define SIZE 256    //Максимальный размер сообщения
#define MSG_PRIO 1  //Приоритет обычных сообщений
#define EXIT_PRIO 10    //Приоритет служебного сообщения для завершения обмена

#endif /* SETTINGS_H_*/