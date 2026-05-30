#include <stdio.h>  //Для потоков ввода-вывода
#include <stdlib.h> //Для генератора чисел
#include <unistd.h> //Системные вызовы sleep(), getpid()
#include <signal.h> //Для сигнала прерывания
#include <time.h>   //Получение времени, также для генератора чисел
#include <semaphore.h>  //Для POSIX-семафоров
#include <fcntl.h>      //Для определения констант при создании семафора
#include <sys/stat.h>   //Для определения прав доступа
#include <string.h> //Для работы со строками
#include <sys/wait.h>   //Для функции wait()

#define FILENAME "data.txt"
#define MAX_LINES 256   //Максимальное количество прочитанных из файла строк
#define MAX_LEN   128   //Максимальная длина строки

static int stop = 0;
void listener(int sig) { stop = 1; }

void producer(sem_t *sem) {
    srand(time(NULL) ^ getpid());   //Задание начальной точки генератора чисел
    while (!stop) {
        sem_wait(sem);  //Уменьшить значение семафора на 1 (блокировка)
        FILE *fd = fopen(FILENAME, "a"); //Открытие файла для добавления строк
        if (fd == NULL){
            perror("File opening error");
            sem_post(sem);  //Увеличить значение семафора на 1 (освобождение) в случае ошибки открытия файла
            break;
        }
        int count = rand() % 8 + 3; //Получение случайного количества чисел
        fprintf(fd, "0:");   //Условная метка строки, которая ещё не была обработана
        for (int i = 0; i < count; i++){
            fprintf(fd, "%d ", rand() % 100);    //Запись в файл случайных чисел
        }
        fprintf(fd, "\n");
        fclose(fd);  //Сохранение и закрытие файла
        printf(">Producer added line\n");
        sem_post(sem);  //Увеличить значение семафора на 1 (освобождение)
        sleep(rand()%3);
    }
    return;
}

void consumer(sem_t *sem) {
    while (!stop) {
        sem_wait(sem);
        FILE *fd = fopen(FILENAME, "r");    //Открытие файла для чтения
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
            printf(">Consumer PID = %d\t File = %s\n", getpid(), FILENAME);
            printf(">Line: %s", lines[i] + 2);
            printf(">MIN = %d\tMAX = %d\n\n", min, max);
            lines[i][0] = '1';  //Установка отметки, что строка обработана
        }
        fd = fopen(FILENAME, "w");  //Открытие файла для записи
        if (fd){    //Перезапись всех строк в файл
            for (int i = 0; i < line_count; i++){
                fputs(lines[i], fd);
            }
            fclose(fd); //Закрытие файла
        }
        sem_post(sem);
        sleep(rand() % 15);
    }
    return;
}

int main() {
    signal(SIGINT, listener);    //Обработка сигнала прерывания ^C
    FILE *fd = fopen(FILENAME, "a");    //Открытие файла, чтобы создать если его до этого не было
    if (fd == NULL){
        perror("fopen");
        return 1;
    }
    fclose(fd);
    sem_t *sem = sem_open("/example_sem", O_CREAT, 0666, 1);    //Создание семафора, как файл в каталоге /dev/shm/sem.example_sem
    if (sem == SEM_FAILED){ //Обработка ошибки создания
        perror("sem_open");
        return 1;
    }

    pid_t pid = fork(); //Порождение дочернего процесса
    if (pid < 0){   //Если неудача - завершение программы
        perror("fork");
        sem_close(sem);
        sem_unlink("/example_sem");
        return 1;
    }
    if (pid == 0) { //Для потомка
        printf(">Consumer started. PID=%d\n", getpid());
        consumer(sem);
        sem_close(sem);
        return 0;
    }
    else {  //Для родителя
        printf(">Producer started. PID=%d\n", getpid());
        producer(sem);
        kill(pid, SIGINT);  //Отправка дочернему процессу сигнала прерывания
        wait(NULL); //Ожидание пока дочерний процесс завершиться
        sem_close(sem); //Закрыть семафор
        sem_unlink("/example_sem"); //Удалить семафор
        printf(">Program finished\n");
    }
    return 0;
}