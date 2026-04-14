#include <stdlib.h> //for strtod()
#include <stdio.h>  //input & output

int main (int argc, char *argv[]){
    printf("Glued string: ");
    for (int i = 1; i < argc; i++){
        printf("%s", argv[i]);
    }
    printf("\n");
    return 0;
}