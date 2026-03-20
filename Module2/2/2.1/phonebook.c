#include "phonebook.h"

/* typedef struct {
    char phoneNumber[16];
    char name[24];
    char surname[24];
    char workPlace[24];
    char job[24];
    char email[32];
} Contact; */

short addContact(Contact* ptr){ //+- доделано
    printf("-= Adding a contact menu =-\n");
    printf("Enter phone number: (Example: +78005554254)\n");
    scanf("%s", ptr->phoneNumber);

    printf("Enter name: (Example: Ivan)\n");
    scanf("%s", ptr->name);

    printf("Enter surname: (Example: Ivanov)\n");
    scanf("%s", ptr->surname);

    while(getchar() != '\n');   //Очистка буфера потока ввода после scanf

    printf("Enter workplace: (Example: Office) (Optional)\n");
    fgets(ptr->workPlace, sizeof(ptr->workPlace), stdin); //Запись из потока ввода в переменную
    ptr->workPlace[strcspn(ptr->workPlace, "\n")] = 0;  //Удаление символа \n из массива символов

    printf("Enter job: (Example: Manager) (Optional)\n");
    fgets(ptr->job, sizeof(ptr->job), stdin); //Запись из потока ввода в переменную
    ptr->job[strcspn(ptr->job, "\n")] = 0;  //Удаление символа \n из массива символов

    printf("Enter email: (Example: my@example.com) (Optional)\n");
    fgets(ptr->email, sizeof(ptr->email), stdin); //Запись из потока ввода в переменную
    ptr->email[strcspn(ptr->email, "\n")] = 0;  //Удаление символа \n из массива символов

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

short printContact(Contact *index, const int id){
    printf("-=Information about contact with ID:%d =-\n", id);
    printf("phone: %s \t", index->phoneNumber);
    printf("name: %s \t", index->name);
    printf("surname: %s \t", index->surname);
    printf("work place: %s \t", index->workPlace);
    printf("job: %s \t", index->job);
    printf("email: %s \n", index->email);
    return 0;
}

short printPhonebook(Contact *phonebook, int length){
    //printf("length %d   %s \n", length, phonebook->phoneNumber);
    //printf("Debug phone: %s \n", phonebook[1].phoneNumber);
    for (int i = 0; i < length; i++){
        short plchldr = printContact(&phonebook[i], i + 1 );

        if (plchldr != 0){
            printf("ALARM: PRINTER BROKE!!");
            return 1;
        }
    }
    return 0;
}