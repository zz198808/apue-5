#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
void * thread_func1(void *args)
{
    printf("thread 1 returning\n");
    return ((void *)1);
}

void * thread_func2(void *args)
{
    printf("thread 2 exiting\n");
    pthread_exit((void *)2);
}

int main(void)
{
    int err;
    pthread_t tid1, tid2;
    void *tret;

    err = pthread_create(&tid1, NULL, &thread_func1, NULL);
    if(err)
    {
        printf("cannot create thread 1\n");
    }
    err = pthread_create(&tid2, NULL, &thread_func2, NULL);
    if(err)
    {
        printf("cannot create thread 2\n");
    }

    err = pthread_join(tid1, &tret);
    if(err)
    {
        printf("thread 1 join error\n");
    }
    printf("thread 1 return code:%d\n", *((int *)tret));

    err = pthread_join(tid2, &tret);
    if(err)
    {
        printf("cannot join thread 2\n");
    }
    printf("thread 2 return code:%d\n", *((int *)tret));
    exit(0);
}
