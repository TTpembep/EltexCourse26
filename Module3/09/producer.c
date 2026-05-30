#include <stdio.h>  //Для потоков ввода-вывода
#include <stdlib.h> //Для генератора чисел
#include <unistd.h> //Системные вызовы sleep(), getpid()
#include <signal.h> //Для сигнала прерывания
#include <time.h>   //Получение времени, также для генератора чисел
#include <semaphore.h>  //Для POSIX-семафоров
#include <fcntl.h>      //Для определения констант при создании семафора
#include <sys/stat.h>   //Для определения прав доступа

static int stop = 0;
void listener(int sig) { stop = 1; }

int main(int argc, char *argv[]) {
    if (argc != 2){ //Проверка количества аргументов при запуске
        printf(">Usage: ./producer <filename.txt>\n");
        return 1;
    }
    signal(SIGINT, listener);   //Обработка сигнала прерывания ^C
    char *filename = argv[1];   //Получение названия файла из агрумента запуска
    srand(time(NULL) ^ getpid());       //Задание начальной точки генератора чисел

    sem_t *sem = sem_open("/example_sem", O_CREAT, 0666, 1);    //Создание семафора, как файл в каталоге /dev/shm/sem.example_sem
    if (sem == SEM_FAILED){ //Обработка ошибки создания
        perror("sem_open");
        return 1;
    }
    printf(">Producer started.\tPID = %d\tFile = %s\n", getpid(), filename);

    while (!stop){
        sem_wait(sem);  //Уменьшить значение семафора на 1 (блокировка)
        FILE *fd = fopen(filename, "a"); //Открытие файла для добавления строк
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
        printf(">Added line\n");
        sem_post(sem);  //Увеличить значение семафора на 1 (освобождение)
        sleep(rand()%3);
    }
    printf(">Producer stopping.\n");
    sem_unlink("/example_sem"); //Удалить семафор
    return 0;
}