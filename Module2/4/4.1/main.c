#include "phonebook.h"

int main() {
    contact *phonebook = NULL;  //Создание указателя на структуру, которая ещё не была разыменована
    int option = 1; //Переменная для switch-case

    while (option != 0) {
        printf("-= PhoneBook console =-\n");
        printf("1. Add contact\n2. Edit contact\n3. Delete contact\n4. Print phonebook\n0. Exit\n");
        printf("Enter option: ");

        scanf("%d", &option);
        while(getchar() != '\n');   //Очистка буфера потока ввода после scanf

        switch(option)
        {
            case 1: //Добавление контакта
                addMenu(&phonebook);
                printf("Operation done successfully!\n");
                break;

            case 2: //Изменение контакта
                editContact(phonebook);
                break;

            case 3: //Удаление контакта
                deleteMenu(&phonebook);
                printf("Operation done successfully!\n");
                break;

            case 4: //Вывод всех контактов
                printPhonebook(phonebook);
                break;

            case 0: //Завершение программы
                option = 0;
                break;

            default:    //Обработка ввода значений
                printf("Wrong option, try again.\n");
                break;
        }
    }
    freePhonebook(phonebook);   //Освобождение памяти
    return 0;
}