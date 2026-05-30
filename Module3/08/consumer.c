#include <stdio.h>  //Для потоков ввода-вывода
#include <stdlib.h> //Для генератора чисел
#include <unistd.h> //Системные вызовы sleep(), getpid()
#include <sys/ipc.h>//Для IPC (Межпроцессное взаимодействие)
#include <sys/sem.h>//Семафоры System V
#include <signal.h> //Для сигнала прерывания
#include <string.h> //Для работы со строками

#define MAX_LINES 256  //Максимальное количество прочитанных из файла строк
#define MAX_LEN   32  //Максимальная длина строки

static int stop = 0;
void listener(int sig) { stop = 1; }

union semun {
    int val;                /* значение для SETVAL */
    struct semid_ds *buf;   /* буферы для  IPC_STAT, IPC_SET */
    unsigned short *array;  /* массивы для GETALL, SETALL */
};

int main(int argc, char *argv[]) {
    if (argc != 2){ //Проверка количества аргументов при запуске
        printf(">Usage: ./producer <filename.txt>\n");
        return 1;
    }
    signal(SIGINT, listener);   //Обработка сигнала прерывания ^C
    char *filename = argv[1];
    key_t key;
    int semid;

    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock = {0, 1, 0};

    key = ftok(filename, 1);
    semid = semget(key, 1, 0666);
    if (semid == -1){
        perror("semget");
        return 1;
    }
    printf(">Consumer started.\tPID=%d\tFile=%s\n", getpid(), filename);

    while (!stop){
        semop(semid, &lock, 1); //Блокировка семафора
        FILE *fd = fopen(filename, "r");    //Открытие файла для чтения
        if (fd == NULL){
            semop(semid, &unlock, 1);
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
        semop(semid, &unlock, 1);   //Освобождение семафора
        sleep(rand()%5);
    }
    return 0;
}