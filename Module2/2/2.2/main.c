#include <stdio.h>
#include "calc.h"

int main(){
    printf(" -= Calculator menu =-\n");
    //Почему из-за ^этой строки^ код начинает выглядит сгенерированным. Я просто люблю оформление...
    short action = 1;       
    while (action != 0){
        double result, num1, num2;
        printf("1. Addition;\n");
        printf("2. Substraction;\n");
        printf("3. Multiplication;\n");
        printf("4. Divsion;\n");
        printf("5. Exponentation;\n");
        printf("6. Taking the root;\n");
        printf("0. Exit application.\n");

        printf("Choose action: ");
        scanf("%hd", &action);
        switch(action){
            case 1: {
                printf("Enter first number: ");
                scanf("%lf", &num1);
                printf("Enter second number: ");
                scanf("%lf", &num2);
                result = calcSum(num1, num2);
                printf("Result: %.3lf\n", result);  //Вывод результата с точность 3 знака после "."
                break;
            }
            default:{
                action = 0;
            }
        }
    }
    return 0;
}