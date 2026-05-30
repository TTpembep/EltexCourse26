#include <stdio.h>  //Для потоков ввода-вывода
#include <stdlib.h> //Для генератора чисел
#include <unistd.h> //Системные вызовы sleep(), getpid()
#include <signal.h> //Для сигнала прерывания
#include <time.h>   //Получение времени, также для генератора чисел
//#include <sys/ipc.h>//Для IPC (Межпроцессное взаимодействие)
//#include <sys/shm.h>//Общая память System V
#include <sys/wait.h>   //Для функции wait()
#include <fcntl.h>  //Для определения констант при создании объекта разделяемой памяти
#include <sys/mman.h>   //Для POSIX-разделямой памяти
#include <sys/stat.h>   //Для определения прав доступ

#define MAX_NUMBERS 20      
#define SHM_NAME "/example_sh_memory"

static int stop = 0;
void listener(int sig) { stop = 1; }

typedef struct{
    int state;  //Состояние текущего цикла, чтобы отделить работу процессов
    int count;  //Количество сгенерированных чисел
    int numbers[MAX_NUMBERS];   //Сгенерированные числа
    int min;    //Минимальное из них
    int max;    //Максимальное из них
    int processed;  //Подсчёт количества обработанных наборов данных
} SharedData;

int main() {
    signal(SIGINT, listener);    //Обработка сигнала прерывания ^C

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);    //Создание объекта разделяемой памяти
    if (shm_fd == -1){
        perror("shm_open");
        return 1;
    }
    if (ftruncate(shm_fd, sizeof(SharedData)) == -1) {  //Задание размера разделяемой памяти
        perror("ftruncate");
        return 1;
    }
    SharedData *shm = mmap( NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);   //Подключение сегмента разделяемой памяти к адресному пространству процесса
    if (shm == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    shm->state = 0; //Явное задание нулевых значений
    shm->processed = 0;
    
    pid_t pid = fork(); //Порождение дочернего процесса
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {   //Дочерний процесс
        while (!stop) {
            if (shm->state == 1) {  //Если состояние 1 - приступить к обработке
                int min = shm->numbers[0];
                int max = shm->numbers[0];
                for (int i = 1; i < shm->count; i++) {  //Нахождение min и max чисел в массиве
                    if (shm->numbers[i] < min) { min = shm->numbers[i]; }
                    if (shm->numbers[i] > max) { max = shm->numbers[i]; }
                }
                shm->min = min;
                shm->max = max;
                shm->state = 2; //Переключение состояния, чтобы продолжил родительский процесс
            }
            sleep(1);
        }
        munmap(shm, sizeof(SharedData));
        close(shm_fd);
        return 0;
    }
    srand(time(NULL));  //Родительский процесс
    while (!stop){
        while (shm->state != 0 && !stop) { sleep(1); }
        if (stop) { break; }
        shm->count = rand() % 10 + 3;   //Случайное количество чисел

        printf("\n>Generated: ");
        for (int i = 0; i < shm->count; i++){
            shm->numbers[i] = rand() % 100; //Случайные числа
            printf("%d ", shm->numbers[i]);
        } printf("\n");

        shm->state = 1; //Переключение состояния
        while (shm->state != 2 && !stop) { sleep(1); }
        if (stop) { break; }
        printf("MIN = %d\t", shm->min);
        printf("MAX = %d\n", shm->max);
        shm->processed++;   //Подсчёт количества обработанных наборов данных
        shm->state = 0;
        sleep(1);
    }

    kill(pid, SIGINT);  //Сигнал прерывания для дочернего процесса
    wait(NULL); //Ожидание заверщения дочернего процесса
    printf("\n>Processed sets: %d\n",shm->processed);   //Вывод количества обработанных наборов данных
    munmap(shm, sizeof(SharedData));    //Отключение сегмента от адресного пространства процесса
    close(shm_fd);  //Закрытие дискриптора разделяемой памяти
    shm_unlink(SHM_NAME);   //Удаление объекта разделяемой памяти (сегмент существует, пока не отключен от всех процессов)
    return 0;
}