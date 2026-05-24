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

        switch(action){         //Затем переход в соответствующий случай
            case 1: {           //Формат вывода задаётся в основном файле
                numberScan(&num1, &num2);               //Функция для получения чисел от пользователя
                result = calcSum(num1, num2);           //Логика вычисления вынесена во внешнюю функцию
                printf("\tResult: %.2lf\n", result);    //Форматирование результата, например с точность 2 знака после "."
                break;
            }
            case 2: {
                numberScan(&num1, &num2);
                result = calcSub(num1, num2);
                printf("\tResult: %.2lf\n", result);
                break;
            }
            case 3: {
                numberScan(&num1, &num2);
                result = calcMul(num1, num2);
                printf("\tResult: %.2lf\n", result);
                break;
            }
            case 4: {
                numberScan(&num1, &num2);
                result = calcDiv(num1, num2);
                printf("\tResult: %.2lf\n", result);
                break;
            }
            case 5: {
                numberScan(&num1, NULL);
                result = calcExp(num1);
                printf("\tResult: %.2lf\n", result);
                break;
            }
            case 6: {
                numberScan(&num1, NULL);
                result = calcRoot(num1);
                printf("\tResult: %.2lf\n", result);
                break;
            }
            default:{
                action = 0;
            }
        }
    }
    return 0;
}