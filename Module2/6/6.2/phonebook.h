#ifndef PHONEBOOK_H_
#define PHONEBOOK_H_

#include <stdio.h>  //Для работы с потокам ввода/вывода
#include <string.h>
#include <stdarg.h> //Для функций с переменным количеством параметров
#include <stdlib.h> //Для работы с динамической памятью

typedef struct {
    char phoneNumber[13];
    char name[20];
    char surname[20];
    char workPlace[20];
    char job[20];
    char email[30];
} contact;

int addContact(contact* ptr);
int addMenu(contact **phonebook, int size); //Передача указателя на указатель

void editContact(contact *phonebook, int size);

void deleteContact(contact *phonebook, int count, ...);
void deleteMenu(contact *phonebook, int size);

void printContact(contact *index, const int id);
void printPhonebook(contact *phonebook, int size);

#endif /* PHONEBOOK_H_ */