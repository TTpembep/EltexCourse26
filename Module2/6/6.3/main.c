#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void numberScan(double* num1, double* num2){
    printf("Enter first number: ");
    while(scanf("%lf", num1) != 1){
        printf("Not a number, try again: ");
        while(getchar() != '\n');
    }
    if(num2 != NULL){
        printf("Enter second number: ");
        while(scanf("%lf", num2) != 1){
            printf("Not a number, try again: ");
            while(getchar() != '\n');
        }
    }
}

int main(){
    void *handles[6];

    double (*operations[4])(double, double);
    double (*singleOperations[2])(double);

    handles[0] = dlopen("./libs/libsum.so", RTLD_LAZY);
    handles[1] = dlopen("./libs/libsub.so", RTLD_LAZY);
    handles[2] = dlopen("./libs/libmul.so", RTLD_LAZY);
    handles[3] = dlopen("./libs/libdiv.so", RTLD_LAZY);
    handles[4] = dlopen("./libs/libexp.so", RTLD_LAZY);
    handles[5] = dlopen("./libs/libroot.so", RTLD_LAZY);

    if(!handles[0] || !handles[1] || !handles[2] || !handles[3] || !handles[4] || !handles[5]){
        printf("%s\n", dlerror());
        return 1;
    }

    operations[0] = dlsym(handles[0], "operation");
    operations[1] = dlsym(handles[1], "operation");
    operations[2] = dlsym(handles[2], "operation");
    operations[3] = dlsym(handles[3], "operation");

    singleOperations[0] = dlsym(handles[4], "singleOperation");
    singleOperations[1] = dlsym(handles[5], "singleOperation");

    short action = 1;
    double result, num1, num2;
    while(action != 0){
        printf("\n-= Calculator menu =-\n");
        printf("1. Addition\n");
        printf("2. Subtraction\n");
        printf("3. Multiplication\n");
        printf("4. Division\n");
        printf("5. Exponent\n");
        printf("6. Root\n");
        printf("0. Exit\n");
        printf("Choose action: ");
        scanf("%hd", &action);

        if(action > 0 && action < 5){
            numberScan(&num1, &num2);
            result = operations[action - 1](num1, num2);
            printf("Result: %.2lf\n", result);
        }else if(action > 4 && action < 7){
            numberScan(&num1, NULL);
            result = singleOperations[action - 5](num1);
            printf("Result: %.2lf\n", result);
        }
    }
    for(int i = 0; i < 6; i++){
        dlclose(handles[i]);
    }
    return 0;
}