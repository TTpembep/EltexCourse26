#include "settings.h"

int main(void){
    mqd_t send_q, recv_q;   //Дискрипторы для очередей сообщений
    struct mq_attr queue_attr;  //Задание атрибутов очереди
    queue_attr.mq_maxmsg = 10;
    queue_attr.mq_msgsize = SIZE;

    if (mq_unlink(Q1) != -1) printf("There was queue1 before\n");   //Удаление заданной очереди, если она существует
    if (mq_unlink(Q2) != -1) printf("There was queue2 before\n");

    send_q = mq_open(Q1, O_CREAT | O_RDWR, 0666, &queue_attr);  //Создание очередей сообщений
    recv_q = mq_open(Q2, O_CREAT | O_RDWR, 0666, &queue_attr);
    if (send_q == (mqd_t)-1 || recv_q == (mqd_t)-1) {   //Проверка что очереди успешно открыты
        perror("Creating queue error");
        return -1;
    }
    char buf[SIZE]; //Буфер для сообщений
    unsigned int prio = MSG_PRIO;   //Переменная для задания приоритета сообщения
    size_t msgLength;
    
    printf("\t>Chat started.\n");
    while (1) {
        printf("Enter message: ");  //Обработка сообщения от пользователя
        fflush(stdout);
        if (!fgets(buf, SIZE, stdin)) break;
        buf[strcspn(buf, "\n")] = '\0';

        if (strcmp(buf, "shutdown") == 0) { //Задание приориета для завершения обмена сообщениями
            prio = EXIT_PRIO;
        }
        if (mq_send(send_q, buf, strlen(buf), prio) == -1){ //Отправка сообщения
            perror("Sending message error");
            break;
        }
        printf("\t>Message sent.\n");\

        if (prio == EXIT_PRIO) {            //Обработка служебного слова для завершения обмена
            printf("Connection closed.\n"); //В случае отправки "shutdown"
            break;
        }
        printf("\t>Waiting for other side.\n");
        if ((msgLength = mq_receive(recv_q, buf, SIZE, &prio)) == -1) {   //Приём сообщения
            perror("mq_receive");
            break;
        }
        buf[msgLength] = '\0'; //Добавление нулевого значения в конце полученного сообщения, чтобы не отображать мусорные значения
        
        if (prio == EXIT_PRIO) {            //Обработка служебного слова для завершения обмена
            printf("Connection closed.\n"); //В случае получения "shutdown"
            break;
        }
        printf("Recieved message: %s\n", buf);  //Вывод полученного сообщения пользователю
    }
    if (mq_close(send_q) == -1) perror("Closing queue1 error"); //Закрытие дискриптора очереди
    if (mq_close(recv_q) == -1) perror("Closing queue2 error");
    if (mq_unlink(Q1) == -1) perror("Removing queue1 error");   //Удаление очереди сообщений
    if (mq_unlink(Q2) == -1) perror("Removing queue2 error");
    return 0;
}