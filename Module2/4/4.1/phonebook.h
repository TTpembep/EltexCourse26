#ifndef PHONEBOOK_H_
#define PHONEBOOK_H_

#include <stdio.h>  //Для работы с потокам ввода/вывода
#include <string.h>
#include <stdarg.h> //Для функций с переменным количеством параметров
#include <stdlib.h> //Для работы с динамической памятью

typedef struct contact {
    char phoneNumber[13];
    char name[20];
    char surname[20];
    char workPlace[20];
    char job[20];
    char email[30];

    struct contact *prev;
    struct contact *next;

} contact;

void addContact(contact *ptr);
void addMenu(contact **phonebook);  //Передача указателя на указатель

void editContact(contact *phonebook);

void deleteContact(contact **phonebook, contact *node);
void deleteMenu(contact **phonebook);

void printContact(contact *index, const int id);
void printPhonebook(contact *phonebook);

void freePhonebook(contact *phonebook); //Функция для освобождения памяти

#endif