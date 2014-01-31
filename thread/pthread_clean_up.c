#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void cleanup(void *arg)
{
    printf("clean up :%s\n", (char *)arg);
}

void * thread_func1(void *arg)
{
    printf("thread 1 start\n");
    pthread_cleanup_push(cleanup, "thread 1 first handler");
    pthread_cleanup_push(cleanup, "thread 1 second handler");
    printf("thread 1 push complete\n");
    if(arg)
    {
        return ((void *)1);
    }

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return ((void *)1);
}

void * thread_func2(void *arg)
{
    printf("thread 2 start\n");
    pthread_cleanup_push(cleanup, "thread 2 first handler");
    pthread_cleanup_push(cleanup, "thread 2 second handler");
    printf("thread 2 push complete\n");
    if(arg)
    {
        pthread_exit((void *)2);
    }

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    pthread_exit((void *)2);


}

int main(int argc, char *argv[])
{
    int err;
    pthread_t tid1, tid2;
    void *tret;

    err = pthread_create(&tid1, NULL, thread_func1, (void *)1);
    if(err)
    {
        printf("error while creating thread 1\n");
    }

    err = pthread_create(&tid2, NULL, thread_func2, (void *)1);
    if(err)
    {
        printf("error while creating thread 2\n");
    }

    err = pthread_join(tid1, &tret);

    if(err)
    {
        printf("error while joining thread 1\n");
    }
    printf("thread 1 exit code :%d\n", (intptr_t)tret);

    err = pthread_join(tid2, &tret);
    if(err)
    {
        printf("error while joining thread 2\n");
    }
    printf("thread 2 exit code:%d\n", (intptr_t)tret);
    exit(0);
}
