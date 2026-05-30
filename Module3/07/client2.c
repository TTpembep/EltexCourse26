#include "settings.h"

int main(void) {
    mqd_t send_q, recv_q;   //Дискрипторы для очередей сообщений
    recv_q = mq_open(Q1, O_RDWR);   //Открытие очередей сообщений
    send_q = mq_open(Q2, O_RDWR);   //Для второго клиента очереди приёма и отправки назначены наоброт (относительно клиента1)
    if (send_q == (mqd_t)-1 || recv_q == (mqd_t)-1) {   //Проверка что очереди успешно открыты
        perror("Creating queue error");
        return -1;
    }
    char buf[SIZE]; //Буфер для сообщений
    unsigned int prio = MSG_PRIO;
    size_t msgLength;
    printf("\t>Chat started.\n");
    while (1) {
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
        printf("\t>Message sent.\n");
        
        if (prio == EXIT_PRIO) {            //Обработка служебного слова для завершения обмена
            printf("Connection closed.\n"); //В случае отправки "shutdown"
            break;
        }
    }
    if (mq_close(send_q) == -1) perror("Closing queue1 error"); //Закрытие дискриптора очереди
    if (mq_close(recv_q) == -1) perror("Closing queue2 error");
    return 0;
}