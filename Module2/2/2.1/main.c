#include "phonebook.h"

int main(){
    int size = 10;   //Значение максимального количества записей в книге
    contact *phonebook = calloc(size, sizeof(contact));  //Выделение памяти
    if (phonebook == NULL){ //Обработка ошибки выделения памяти
        printf("Memory allocation error\n");
        return 1;
    }
    int option = 1; //Переменная для switch-case

    while (option !=0){
        printf("-= PhoneBook console =-\n");
        printf("1. Add contact\n2. Edit contact\n3. Delete contact\n4. Print phonebook\n0. Exit\n");
        printf("Enter option: ");

        scanf("%d", &option);
        while(getchar() != '\n');   //Очистка буфера потока ввода после scanf

        switch(option)
        {
            case 1: //Добавление контакта
                size = addMenu(&phonebook, size);

                if (size == -1) {
                    printf("Memory reallocation error\n");
                    free(phonebook);    //Освобождение памяти "на всякий случай"
                    return 1;   //Завершение программы
                }
                printf("Operation done successfully!\n");
                break;

            case 2: //Изменение контакта
                editContact(&phonebook[0], size);
                //printf("Operation done successfully!\n");
                break;

            case 3: //Удаление контакта
                deleteMenu(&phonebook[0], size);
                printf("Operation done successfully!\n");
                break;

            case 4: //Вывод всех контактов
                printPhonebook(&phonebook[0], size);
                break;
            
            case 0: //Завершение программы
                option = 0;
                break;
            
            default:    //Обработка ввода значений
                printf("Wrong option, try again.\n");
                break;
        }
    }
    free (phonebook);   //Освобождение памяти
    return 0;
}