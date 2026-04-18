#include <stdio.h>  //i/o streams
#include <unistd.h> //for wait(sec)
#include <signal.h>
//#include <fcntl.h>  //file control options

static int wait = 3;
void listener (int sig) {
    //printf("listener:stop, %d\n", wait);
    wait -= 1;
    return;
}

int main(){
    signal(SIGINT, listener);
    signal(SIGQUIT, listener);
    signal(SIGKILL, listener);
    signal(SIGTERM, listener);
    signal(SIGTSTP, listener);

    signal(SIGABRT, listener);
    signal(SIGCONT, listener);
    int counter = 0;
    char string[32];
    int tempWait = wait;
    while (1) {
        FILE * fp = fopen("file.txt", "a");
        if(fp){
            sprintf(string, "%d ", counter++);
            fputs(string, fp);
            if (tempWait != wait){
                fputs("Interrupt ", fp);
                tempWait = wait;
            }
            if (wait == 0){
                fputs("Closing\n", fp);
                fclose(fp);
                return 0;
            }
            fclose(fp);
            sleep(1);
        }
    } //while(wait);
    return 0;
}