#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void *doit(void *vargp)
{
    sleep(1);
    printf("new thread now performing a task\n");
    return NULL;
}

void *detach(void *vargp)
{
    sleep(1);
    printf("new thread about to detach\n");
    pthread_detach(vargp);
       
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    int ret;

    if (argc < 2) {
        printf("error, expected 2 arguments\n");
        return -1;
    }

    //test to see if a new thread can continue after creating
    //thread exits using pthread_exit
    if (strcmp(argv[1], "1") == 0) {
        printf("main thread creating a new thread\n");
        pthread_create(&tid, NULL, doit, NULL);
        printf("main thread exiting\n");
        pthread_exit(NULL);
        printf("after thread\n");
        exit(0);
        return 0;
    }
    else if (strcmp(argv[1], "2") == 0) {
        printf("main thread creating a new thread\n");
        pthread_create(&tid, NULL, doit, NULL);
        printf("main thread about to detach new thread\n");
        ret = pthread_detach(tid);
        if (ret) {
            printf("ERROR: return code from pthread_detach() is %d\n", ret);
            exit(-1);
        }
        //pthread_create(&tid, NULL, detach, NULL);
    }
        
    
}
