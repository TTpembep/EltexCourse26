#include <stdlib.h> //for strtod()
#include <stdio.h>  //input & output

int main (int argc, char *argv[]){
    double num, sum;
    for (int i = 0; i < argc; i++){
        num = strtod(argv[i], NULL);
        if (num != 0){
            sum += num;
        }
        else {printf("Not numbers: %s \t", argv[i]);}
    }
    printf("\nSum = %.2f\n", sum);
    return 0;
}