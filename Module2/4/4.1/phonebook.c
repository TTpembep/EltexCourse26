#include "phonebook.h"

void addContact(contact *ptr) { //Функция добавления контакта
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

    return;
}

void addMenu(contact **phonebook) {
    contact *newNode = (contact*)calloc(1, sizeof(contact));    //Выделение памяти для нового узла списка

    if (newNode == NULL) {  //Проверка что память выделилась корректно
        printf("Memory allocation error.\n");
        return;
    }

    addContact(newNode);    //Вывоз функции для получения данных от пользователя

    if (*phonebook == NULL) {   //Если список пуст, то указатель равен NULL
        *phonebook = newNode;   //Тогда указываем на новый узел списка
        return;
    }
    //Проверка условий для сортировки контакта по фамилии
    contact *current = *phonebook;  //Временный указатель
    while (current != NULL && strcmp(newNode->surname, current->surname) > 0) { //Сравнение фамилий, порядок сортировки A-Z a-z
        current = current->next;
    }

    if (current == *phonebook) {    //Случай вставки в начало
        newNode->next = *phonebook; //Указываем новым элементом на начало списка
        (*phonebook)->prev = newNode;   //Бывшему началу списка задаём новый узел как предыдущий
        *phonebook = newNode;   //И задаём новый узел как указатель на весь список
        return;
    }
    if (current == NULL) {  //Случай вставки в конец
        contact *tail = *phonebook; //Нужно ещё раз пройти список, чтобы узнать указатель на предыдущий элемент
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = newNode;   //Связываем последний элемент с новым
        newNode->prev = tail;   //И обратно
        return;
    }
    //Случай вставки в середину
    contact *previous = current->prev;  //Сохраняем указатель предыдущий элемент, чтобы не потерять его

    previous->next = newNode;   //Создание связи между новым элементом и предыдущим
    newNode->prev = previous;

    newNode->next = current;    //Создание связи между новым элементов и следующим
    current->prev = newNode;
    return;
}

void editContact(contact *phonebook) {  //Функция изменения контакта
    if (phonebook == NULL) {    //Проверка что список не пуст
        printf("Phonebook is empty.\n");
        return;
    }

    int id; //Обработка пользовательского ввода
    printf("Enter ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Not a number, try again: ");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');

    contact *current = phonebook;   //Получение указателя на начало списка
    int index = 0;
    while (current != NULL && index < id) { //Прохождение по списку до указанного id
        current = current->next;
        index++;
    }
    if (current == NULL) {  //Проверка создан ли контакт с соответствующим id
        printf("Contact not found.\n");
        return;
    }

    int option = -1;
    while (option != 0) {
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

void deleteContact(contact **phonebook, contact *node) {    //Функция удаления контакта
    if (node == NULL) { return; }   //Возврат если узел пуст
    if (node->prev != NULL) {   //Если есть указатель на предыдущий узел, переназначаем связи между узлами, чтобы не потерять связность
        node->prev->next = node->next;
    }
    else { *phonebook = node->next; }   //Иначе переназначаем начальный элемент списка
    if (node->next != NULL) {   //Аналогично если узел не последний в списке
        node->next->prev = node->prev;
    }
    free(node); //Освобождаем память
    return;
}

void deleteMenu(contact **phonebook) {
    if (*phonebook == NULL) {   //Проверка что список не пустой
        printf("Phonebook is empty.\n");
        return;
    }
    int id; //Обработка пользовательского ввода
    printf("Enter ID to delete:\n");
    if (scanf("%d", &id) != 1) {
        printf("Not a number.\n");
        while(getchar() != '\n');
        return;
    }
    contact *current = *phonebook;  //Поиск узла списка с заданными id
    int index = 0;
    while (current != NULL && index < id) {
        current = current->next;
        index++;
    }
    if (current == NULL) {  //Проверка что такой узел существует
        printf("Contact not found.\n");
        return;
    }
    deleteContact(phonebook, current);  //Вызов функции удаления узла
    return;
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

void printPhonebook(contact *phonebook) {   //Вывод всех контактов
    printf("\t## The Phone Book ##\n");
    contact *current = phonebook;
    int id = 0;
    while (current != NULL) {
        printContact(current, id);
        current = current->next;
        id++;
    }
    if (id == 0) {
        printf("Phonebook is empty.\n");
    }
    return;
}

void freePhonebook(contact *phonebook) {    //Функция для освобождения памяти
    contact *current = phonebook;
    while (current != NULL) {
        contact *temp = current;
        current = current->next;
        free(temp);
    }
    return;
}