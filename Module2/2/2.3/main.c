#include <stdio.h>
#include "calc.h"

void numberScan(double* num1, double* num2){
    printf("Enter first number: ");
    while ( (scanf("%lf", num1) != 1)){  //Обработка ввода символов, отличных от чисел
        printf("Not a number, try again: ");
        while(getchar()!='\n'); //Очистка буфера
    }  

    if (num2 != NULL){  //Проверка, требуется ли получение второго числа
        printf("Enter second number: ");
        while ( (scanf("%lf", num2) != 1)){
            printf("Not a number, try again: ");
            while(getchar()!='\n');
        }
    }
    return;
}

int main(){
    double result, num1, num2;
    //Объявление массивов указателй на фукции, их 2 потому-что разное количество входных параметров
    double (*operations[4])(double, double) = {calcSum, calcSub, calcMul, calcDiv};
    double (*singleOperations[2])(double) = {calcExp, calcRoot};
    unsigned length = sizeof(operations);
    short action = 1;
    while (action != 0){
        result = 0; num1 = 0; num2 = 0;
        printf(" -= Calculator menu =-\n");
        printf("1. Addition;\n");           //Суммирование
        printf("2. Substraction;\n");       //Вычитание
        printf("3. Multiplication;\n");     //Умножение
        printf("4. Divsion;\n");            //Деление
        printf("5. Exponentation;\n");      //Возведение в степень
        printf("6. Square Root;\n");        //Взятие корня
        printf("0. Exit application.\n");   //Выход

        printf("Choose action: ");  //Получение от пользователя какое действие выполнить
        while ( (scanf("%hd", &action) != 1)){  //Обработка ввода символов, отличных от чисел
            printf("Not a number, try again: ");
            while(getchar()!='\n'); //Очистка буфера
        }  

        if (action < 5 && action > 0){  //Использование указателй на функции
            numberScan(&num1, &num2);
            result = operations[action - 1](num1, num2);
        }else if (action < 7 && action > 4){
            numberScan(&num1, NULL);
            result = singleOperations[action - 5](num1);
        }else if (action == 0 ) {
            break;
        }
        printf("\tResult: %.2lf\n", result);
    }
    return 0;
}