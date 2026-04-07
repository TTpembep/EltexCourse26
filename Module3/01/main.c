//#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]){
    //Разделение массива данных и запуск дочернего процесса



    
    //Базовая функция для обработки входных данных
    //Нужно продумать случаи: 42word & 42.12
    //Работает для: word, word42, 42
    int num;
    for (int i=0; i < argc; i++){
        num = atoi(argv[i]);
        if (num != 0){
            printf("%d %d \n", num, num*2);
        }
        else {printf("%s \n", argv[i]);}
    }

    //Добавить ожидание окончания дочернего процесса?

    return 0;
}