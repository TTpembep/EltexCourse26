#include "phonebook.h"

int addContact(contact* ptr){ //Функция добавления контакта
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
    fgets(ptr->job, sizeof(ptr->job), stdin);
    ptr->job[strcspn(ptr->job, "\n")] = 0;

    printf("Enter email: (Example: my@example.com) (Optional)\n");
    fgets(ptr->email, sizeof(ptr->email), stdin);
    ptr->email[strcspn(ptr->email, "\n")] = 0;

    return 0;
}

int addMenu(contact **phonebook, int size){
    for (int i = 0; i < size; i++){
        if ((*phonebook)[i].phoneNumber[0] == '\0'){
            addContact(&(*phonebook)[i]);
            return size;
        }
    }
    printf("Phonebook is full, doubling memory.\n");
    int oldSize = size; //Увеличение размера выделенной памяти вдвое
    size *= 2;  
    contact *temp = realloc(*phonebook, size * sizeof(contact));    //Выделение нового участка памяти
    if (temp == NULL){
        return -1;
    }
    *phonebook = temp;  //Перезапись исходного указателя на новый с увеличенным количеством записей
    memset(&(*phonebook)[oldSize], 0, (size - oldSize) * sizeof(contact));  //Очистка новой части памяти от мусорных данных
    addContact(&(*phonebook)[oldSize]);
    return size;
}

void editContact(contact *phonebook, int size){    //Функция изменения контакта
    int id = -1;
    printf("Enter ID. Range = [0 - %d].\n", size - 1);
    while (1){
        if (scanf("%d", &id) != 1){  //Обработка ввода символов, отличных от чисел
            printf("Not a number, try again: ");
            while(getchar()!='\n'); //Очистка буфера
        }
        else if (id < 0 || id > size){
            printf("Out of range, try again: ");
        }
        else if(phonebook[id].phoneNumber[0] == '\0'){
            printf("Contact is empty, use \"Add contact\" function instead.\n");
            return;
        }
        else {break;}
    }

    int option = -1;
    while (option != 0){
        printf("-= Contact editor =-\t");
        printContact(&phonebook[id], id);
        printf("1. Change number\n2. Change name\n3. Change surname\n4. Change workplace\n5. Change job \n6. Change email \n0. Exit\n");
        printf("Enter option: ");

        if (scanf("%d", &option) != 1){
            printf("Not a number.\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');

        if (option == 0) { return; }

        if (option < 1 || option > 6){
            printf("Wrong option.\n");
            continue;
        }

        //Массив указателей на поля структуры
        char *fields[] = {phonebook[id].phoneNumber, phonebook[id].name, phonebook[id].surname, phonebook[id].workPlace, phonebook[id].job, phonebook[id].email};
        //Массив значений размера полей структуры, так как иначе при обращении к указателю получался размер указателя, а не нужного массива.
        int fieldSizes[] = {sizeof(phonebook[id].phoneNumber), sizeof(phonebook[id].name), sizeof(phonebook[id].surname), sizeof(phonebook[id].workPlace), sizeof(phonebook[id].job), sizeof(phonebook[id].email)};

        char buffer[30];
        printf("Enter new value: (leave blank for no change)\n");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        if (buffer[0] == '\0'){ continue; } //Если строка пустая - пропуск перезаписи

        strncpy(fields[option - 1], buffer, fieldSizes[option - 1]);
        printf("Field updated successfully.\n");
    }
    return;
}

//Пример использования функции с переменным количеством параметров
void deleteContact(contact *phonebook, int count, ...){  //Функция удаления контакта
    va_list args;   //указатель на va_list
    va_start (args, count); //указатель на начало

    for (int i = 0; i < count; i++){
        int id = va_arg(args, int); //Получение значение текущего параметра типа int
        memset(&phonebook[id], 0, sizeof(contact)); //Зануление заданной ячейки памяти
    }

    va_end(args);   //Завершение обработки параметров
    return;
}

void deleteMenu(contact *phonebook, int size){
    int count = -1;
    printf("How many contacts to delete? Min = 0, Maximum = 5.\n");
    while (1){
        if (scanf("%d", &count) != 1){  //Обработка ввода символов, отличных от чисел
            printf("Not a number, try again: ");
            while(getchar()!='\n'); //Очистка буфера
        }
        else if (count < 0 || count > 5){
            printf("Out of range, try again: ");
        }else {break;}
    }

    int ids[5];

    if (count != 0) {printf("Enter IDs. Range = [0 - %d].\n", size - 1);}

    for (int i = 0; i < count; i++){
        while (1){
            if (scanf("%d", &ids[i]) != 1){  //Обработка ввода символов, отличных от чисел
                printf("Not a number, try again: ");
                while(getchar()!='\n'); //Очистка буфера
            }
            else if (ids[i] < 0 || ids[i] > size){
                printf("Out of range, try again: ");
            }else {break;}
        }
    }

    

    switch(count){
        case 1: return (deleteContact(phonebook, 1, ids[0]));
        case 2: return (deleteContact(phonebook, 2, ids[0], ids[1]));
        case 3: return (deleteContact(phonebook, 3, ids[0], ids[1], ids[2]));
        case 4: return (deleteContact(phonebook, 4, ids[0], ids[1], ids[2], ids[3]));
        case 5: return (deleteContact(phonebook, 5, ids[0], ids[1], ids[2], ids[3], ids[4]));
        default: return;
    }
}

void printContact(contact *index, const int id){   //Форматирование вывода отдельного контакта
    printf("-= Information about contact with ID: %d =-\n", id);
    printf("phone: %s \t", index->phoneNumber);
    printf("name: %s \t", index->name);
    printf("surname: %s \t", index->surname);
    printf("workplace: %s \t", index->workPlace);
    printf("job: %s \t", index->job);
    printf("email: %s \n", index->email);
    return;
}

void printPhonebook(contact *phonebook, int size){   //Вывод всех контактов
    printf("\t## The Phone Book ##\n");
    for (int i = 0; i < size; i++){
        if (phonebook[i].phoneNumber[0] == '\0'){   //Проверка пустых элементов массива
            printf("Contact with ID:%d is empty\n", i);
        }
        else {
            printContact(&phonebook[i], i );
        }
    }
    return;
}