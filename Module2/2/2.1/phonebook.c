#include "phonebook.h"

/* typedef struct {
    char phoneNumber[13];
    char name[20];
    char surname[20];
    char workPlace[20];
    char job[20];
    char email[30];
} Contact; */

short addContact(Contact* one){
    /* printf("Enter contact`s data.\n");
    printf("Format: \"number,name,surname,workPlace,job,email\"\n");
    printf("Only phone number, name and surname are reqired. Other parameters are optional.\n");
    printf("Example: +78005554254,Ivan,Ivanov");
    char input[130];
    scanf("%s", input); */
    printf("-= Adding a contact menu =-\n");
    printf("Enter phone number: (Example: +78005554254)\n");
    scanf("%s", one->phoneNumber);

    //while(getchar() != '\n');   //Очистка буфера

    printf("Enter job: (Example: Office worker) (Optional)\n");
    while(getchar() != '\n');   //Очистка буфера
    char buffer[32];   //Буфер для возможности задания пустого поля
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;  //Удаление символа \n из массива символов
        if (strlen(buffer) > 0) {   //Если строка не пустая - копируем в структуру
            strcpy(one->job, buffer);
        } else {
            //one->email[0] = '\0';  //Пустая строка
            //printf("Job skipped\n");
        }
        
    }
    while(getchar() != '\n');   //Очистка буфера

    printf("Enter email: (Example: my@example.com) (Optional)\n");
    while(getchar() != '\n');   //Очистка буфера
    //scanf("%s", one->email);
    //char buffer[32];   //Буфер для возможности задания пустого поля
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;  //Удаление символа \n из массива символов
        if (strlen(buffer) > 0) {   //Если строка не пустая - копируем в структуру
            strcpy(one->email, buffer);
        } else {
            //one->email[0] = '\0';  //Пустая строка
            //printf("Email skipped\n");
        }
        
    }

    return 0;
}

short editContact(void){
    printf("Placeholder\n");
    return 0;
}

short deleteContact(void){
    printf("Placeholder\n");
    return 0;
}

short printContact(void){
    printf("Placeholder\n");
    return 0;
}