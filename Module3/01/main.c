//#include <ctype.h>
#include <stdlib.h> //for atoi()
#include <stdio.h>  //input & output
#include <sys/types.h>  //pid_t
#include <unistd.h> //fork()

int main (int argc, char *argv[]){
    pid_t childId = fork();     //Запуск дочернего процесса
    int i = 0;

    if (childId<0){ //Проверка что процесс успешно создан
        perror("fork failed");
        exit(1);
    }
    else if(childId == 0){
        printf("-= Child`s id: %d =-\n", getpid());
        i++;    //Для разделения вывода. четное или нечетное
    }
    else{
        printf("-= Parent`s id: %d =-\n", getpid());
    }

    double num;
    for (i; i < argc; i=i+2){
        num = strtod(argv[i], NULL);
        if (num != 0){
            printf("%.2f\t%.2f \n", num, num*2);
        }
        else {printf("%s \n", argv[i]);}
    }
    //^^Базовая функция для обработки входных данных
    //В случае: 42word Вывод: 42.00 Не совсем корректно
    //Работает для: word, word3, 16, 13.37
    //Недостаток: Логика создания дочернего процесса смешана с основной логикой программы
    
    wait(NULL);
    return 0;
}