#include <stdio.h>  //for i/o streams
#include <stdlib.h> //for exit(1)
#include <sys/wait.h>   //for wait()
#include <string.h> //for str functions
#include <unistd.h> //for fork(), exec()

#define MAX_LENGTH 256
#define MAX_ARGS 16

int main(){
    char line[MAX_LENGTH];  //Массив символов для строки которую вводит пользователь
    char *args[MAX_ARGS];   //Массив указателей на части массива line, которые используются как аргументы

    while (1){
        printf("\n-= Homemade Command-line interpreter =-\n Syntax: command [OPTION]\n your@command>");

        if (fgets(line, MAX_LENGTH, stdin) == NULL) {
            break;  //Обработка ошибки ввода - завершение программы
        }
        line[strcspn(line, "\n")] = 0;  //Удаление символа переноса строки

        int i = 0;  //Разбиение строки на аргументы
        char *temp = strtok(line, " \t"); //Временное хранение аргумента, разделитель - пробел либо табуляция
        while (temp != NULL && i < MAX_ARGS - 1) {  //Обработка всей строки, пока есть разделители и кол-во аргументов < максимума
            args[i++] = temp;   //Запись указателя в i-тую ячейку, затем инкремент i 
            temp = strtok(NULL, " \t");   //Взятие указателя на следующий аргумент
        }
        args[i] = NULL; //Ограничитель для exec()

        if (args[0] == NULL) { continue; }  //Проверка пустого ввода - продолжение работы
        if (strcmp(args[0], "exit") == 0) { break; }   //Завершение работы командой exit

        pid_t pid = fork(); //Создание дочернего процесса
        if (pid < 0) {  //Обработка ошибки создания дочернего процесса
            printf("fork error\n");
            continue;
        }else if (pid == 0){//Логика дочернего процесса         
            execv(args[0], args);  //Замена образа процесса из текущего каталога
            execvp(args[0], args);  //Замена образа процесса, поиск команды в PATH
            printf("command not found\n");
            exit(1);
        }else{  //Логика родительского процесса
            wait(NULL); //Ожидание завершения дочернего процесса
        }
    }
    return 0;
}