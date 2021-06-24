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

int main(int argc, char **argv)
{
    pthread_t tid;
    printf("main thread creating a new thread\n");
    pthread_create(&tid, NULL, doit, NULL);
    printf("main thread exiting\n");
    pthread_exit(NULL);
    printf("after thread\n");
    exit(0);
}
