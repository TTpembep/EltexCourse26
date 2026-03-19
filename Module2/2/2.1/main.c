#include <stdio.h>
#include "phonebook.h"

int main(){
    printf("-= Welcome to PhoneBook console =-\n");

    Contact phonebook[10];
    int i = 0;
    Contact one = phonebook[i];

    int option = 1;
    while (option !=0){
        printf("Choose an option:\n");
        printf("1. Add contact\n2. Edit contact\n3. Delete contact\n4. Print phonebook\n0. Exit\n");

        scanf("%d", &option);
        short result;
        switch(option)
        {
            case 1:
                result = addContact(&one);
                if (result == 0) {printf("Operation done successfully!\n");}
                else {printf("Someting went wrong.\n");}
                printf("Debug: %s \n", one.phoneNumber);
                printf("Debug: %s \n", one.job);
                printf("Debug: %s \n", one.email);
                break;
            case 2:
                result = editContact();
                if (result == 0) {printf("Operation done successfully!\n");}
                else {printf("Someting went wrong.\n");}
                break;
            case 3:
                result = deleteContact();
                if (result == 0) {printf("Operation done successfully!\n");}
                else {printf("Someting went wrong.\n");}
                break;
            case 4:
                result = printContact();
                if (result == 0) {printf("Operation done successfully!\n");}
                else {printf("Someting went wrong.\n");}
                break;
            default:
                option =0;
        }
    }
    return 0;
}