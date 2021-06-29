#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void *doit(void *vargp)
{
    sleep(1);
    printf("...doit()...\n");
    printf("new thread now performing a task\n");
    return NULL;
}

void *doitAndExit(void *vargp)
{
    sleep(1);
    printf("...doitAndExit()...\n");
    printf("new thread now going to execute exit(0) and main thread will stop\n");
    exit(0);
}

void *detach(void *vargp)
{
    sleep(1); 
    printf("...detach()...\n");
    printf("new thread about to detach\n");
    pthread_detach(pthread_self());
    printf("new thread detached and main has exited\n");
    return NULL;
}

void *detachAndExit(void *vargp)
{
    sleep(1);
    printf("...detachAndExit()...\n");
    printf("new thread about to detach\n");
    pthread_detach(pthread_self());
    printf("new thread detached and will execute exit(0)\n");
    exit(0);
    return NULL;
}


int main(int argc, char *argv[])
{
    printf("...main()...\n");

    pthread_t tid;
    int ret;

    if (argc < 2) {
        printf("error, expected 2 arguments\n");
        return -1;
    }

    if (strcmp(argv[1], "1") == 0) {
        printf("\ntesting if thread will still exist if creating thread exists by pthread_exit()\n\n");
        printf("main thread creating a new thread\n doit functionn");
        pthread_create(&tid, NULL, doit, NULL);
        printf("main thread exiting\n");
        pthread_exit(NULL);
        printf("after thread\n");
        return 0;
    }
    else if (strcmp(argv[1], "2") == 0) {
        printf("\ntesting if detached thread can exist if main calls exit(0)\n\n");
        printf("main thread creating a new thread in detach function\n");
        pthread_create(&tid, NULL, detach, NULL);
        printf("main thread exiting via exit(0)\n");
        exit(0);
    }
    else if (strcmp(argv[1], "3") == 0) {
        printf("\ntesting if a process will continue if one of the normal (not detached)\n" 
               "and non-main threads within it calls exit\n\n");
        printf("main thread creating a new thread in doitAndExit function\n");
        pthread_create(&tid, NULL, doitAndExit, NULL);
        sleep(3);
        //this loop will not execute since the new thread will exit(0)
        int i;
        for (i = 0; i < 25; i++) {
            printf("%d\n", i);
        }
        pthread_exit(NULL);

    }
    else if (strcmp(argv[1], "4") == 0) {
        printf("\ntesting if original process will still continue if a detached thread calls exit(0)\n\n");
        printf("main thread creating a new thread in detachAndExit function\n");
        pthread_create(&tid, NULL, detachAndExit, NULL);
        //sleep(3);
        //this loop does execute`
        int i;
        for (i = 0; i < 25; i++) {
            printf("%d\n", i);
        }
        pthread_exit(NULL);
    } 
    else if (strcmp(argv[1], "5") == 0) {
        printf("\ntesting if the process can continue if the main thread terminates normally\n\n");
        pthread_detach(pthread_self());
        return 0;
    }
    else
        printf("%s not a valid argument\n", argv[1]);
}
