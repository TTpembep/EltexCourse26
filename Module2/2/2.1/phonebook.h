#ifndef PHONEBOOK_H_
#define PHONEBOOK_H_

#include <stdio.h>
#include <string.h>

typedef struct {
    char phoneNumber[13];
    char name[20];
    char surname[20];
    char workPlace[20];
    char job[20];
    char email[30];
} Contact;

short addContact(Contact* one);
short editContact(void);
short deleteContact(void);
short printContact(void);

#endif /* PHONEBOOK_H_ */