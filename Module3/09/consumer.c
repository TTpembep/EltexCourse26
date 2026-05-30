#include <stdio.h>  //Для потоков ввода-вывода
#include <stdlib.h> //Для генератора чисел
#include <unistd.h> //Системные вызовы sleep(), getpid()
#include <signal.h> //Для сигнала прерывания
#include <string.h> //Для работы со строками
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_LINES 256  //Максимальное количество прочитанных из файла строк
#define MAX_LEN   32  //Максимальная длина строки

static int stop = 0;
void listener(int sig) { stop = 1; }

int main(int argc, char *argv[]) {
    if (argc != 2){ //Проверка количества аргументов при запуске
        printf(">Usage: ./producer <filename.txt>\n");
        return 1;
    }
    signal(SIGINT, listener);   //Обработка сигнала прерывания ^C
    char *filename = argv[1];
    sem_t *sem = sem_open("/example_sem", 0);
    if (sem == SEM_FAILED){
        perror("sem_open");
        return 1;
    }
    printf(">Consumer started.\tPID=%d\tFile=%s\n", getpid(), filename);

    while (!stop){
        sem_wait(sem);
        FILE *fd = fopen(filename, "r");    //Открытие файла для чтения
        if (fd == NULL){
            sem_post(sem);
            sleep(1);
            continue;
        }
        char lines[MAX_LINES][MAX_LEN]; //Двумерный массив, для записи каждой строки посимвольно
        int line_count = 0; //Счётчик считанных строк
        while (fgets(lines[line_count], MAX_LEN, fd)){  //Построчное чтение строк
            line_count++;
            if (line_count >= MAX_LINES) break;
        }
        fclose(fd); //Закрытие файла
        for (int i = 0; i < line_count; i++){
            if (lines[i][0] != '0'){    //Пропуск обработанных строк
                continue;
            }
            char temp[MAX_LEN];
            strcpy(temp, lines[i] + 2);
            int min = 101;   //Минимальные и максимальные значения для сравнения
            int max = -1;
            char *token = strtok(temp, " \n");
            while (token){  //Нахождение минимального и максимального значения в строке
                int value = atoi(token);
                if (value < min) min = value;
                if (value > max) max = value;
                token = strtok(NULL, " \n");
            }
            printf(">Consumer PID = %d\t File = %s\n", getpid(), filename);
            printf(">Line: %s", lines[i] + 2);
            printf(">MIN = %d\tMAX = %d\n\n", min, max);
            lines[i][0] = '1';  //Установка отметки, что строка обработана
        }
        fd = fopen(filename, "w");  //Открытие файла для записи
        if (fd){    //Перезапись всех строк в файл
            for (int i = 0; i < line_count; i++){
                fputs(lines[i], fd);
            }
            fclose(fd); //Закрытие файла
        }
        sem_post(sem);
        sleep(rand()%5);
    }
    return 0;
}