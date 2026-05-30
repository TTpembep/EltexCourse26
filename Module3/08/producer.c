#include <stdio.h>  //Для потоков ввода-вывода
#include <stdlib.h> //Для генератора чисел
#include <unistd.h> //Системные вызовы sleep(), getpid()
#include <sys/ipc.h>//Для IPC (Межпроцессное взаимодействие)
#include <sys/sem.h>//Семафоры System V
#include <signal.h> //Для сигнала прерывания
#include <time.h>   //Получение времени, также для генератора чисел

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
    char *filename = argv[1];   //Получение названия файла из агрумента запуска
    key_t key;  //Ключ (имя) семафора
    int semid;  //Идентификатор набора семафоров
    union semun arg;    //Аргументы для системного вызова semctl()

    struct sembuf lock = {0, -1, 0};    //Семафор 0, блокировать(-1), без флагов
    struct sembuf unlock = {0, 1, 0};   //Семафор 0, освободить(+1), без флагов
    srand(time(NULL) ^ getpid());       //Задание начальной точки генератора чисел

    FILE *tmp = fopen(filename, "a");   //Если файла нет он будет создан
    if (!tmp) {                         //Наличие файла необходимо для 
        perror("fopen");                //Корректной работы ftok
        return 1;
    } fclose(tmp);  //Закрытие файла
    key = ftok(filename, 1);    //Преобразование имени файла в ключ для системных вызовов
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {   //Создание семафора с именем из переменной key
        perror("Creating semaphore error");
        return 1;
    }
    //В семафоре 0 установить значение 1
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    printf(">Producer started.\tPID = %d\tFile = %s\n", getpid(), filename);

    while (!stop){
        semop(semid, &lock, 1); //Блокировка семафора
        FILE *fd = fopen(filename, "a"); //Открытие файла для добавления строк
        if (fd == NULL){
            perror("File opening error");
            semop(semid, &unlock, 1);   //Освобождение семафора в случае ошибки открытия файла
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
        semop(semid, &unlock, 1);   //Освобождение семафора
        sleep(rand()%3);
    }
    printf(">Producer stopping.\n");
    semctl(semid, 0, IPC_RMID); //Удалить семафор
    return 0;
}