#include <stdio.h>
#include "phonebook.h"

int main(){
    printf("-= Welcome to PhoneBook console =-\n");

    Contact phonebook[10], *ptr;

    int index = 0;
    short option = 1;
    short result;
    while (option !=0){

        //Прежде чем назначать новый индекс нужно проверить что массив не переполнен, если что его перевыделить память
        ptr = &phonebook[index];

        printf("Choose an option:\n");
        printf("1. Add contact\n2. Edit contact\n3. Delete contact\n4. Print phonebook\n0. Exit\n");

        scanf("%hd", &option); //Раньше здесь был %d и из-за этого затирался первый номер в структуре
        while(getchar() != '\n');   //Очистка буфера потока ввода после scanf

        switch(option)
        {
            case 1:
                result = addContact(ptr);
                if (result == 0) {
                    //printf("Debug phone: %s \n", ptr->phoneNumber);
                    //printf("Debug name: %s \n", ptr->name);
                    //printf("Debug surname: %s \n", ptr->surname);
                    //printf("Debug job: %s \n", ptr->job);
                    //printf("Debug email: %s \n", ptr->email);
                    index++;
                    printf("Operation done successfully!\n");
                }
                else {printf("Someting went wrong.\n");}
                
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
                result = printPhonebook(&phonebook[0], index);
                //result = printContact(phonebook[index]);
                if (result == 0) {printf("Operation done successfully!\n");}
                else {printf("Someting went wrong.\n");}
                break;
            default:
                option = 0;
        }
    }
    return 0;
}