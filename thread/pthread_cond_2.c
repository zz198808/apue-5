#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int i = 0;

void * thread_func1(void *arg)
{
    while(i < 9)
    {
        pthread_mutex_lock(&mux);
        if(i % 3 != 0)
        {
            pthread_cond_wait(&cond, &mux);
            printf("Consumer get i:%d\n", i);
        }
        pthread_mutex_unlock(&mux);
    }
    pthread_exit((void *)0);
}

void * thread_func2(void *arg)
{
    for(i = 0; i <= 9; i++)
    {
        pthread_mutex_lock(&mux);
        printf("Producer set i:%d\n", i);
        if(i % 3 == 0)
        {
            pthread_cond_signal(&cond);
        }
        pthread_mutex_unlock(&mux);
        sleep(1);
    }
    pthread_exit((void *)0);
}

int main(int argc, char *argv[])
{
    pthread_t tid1, tid2;
    int err;

    err = pthread_create(&tid1, NULL, thread_func1, NULL);
    if(err)
    {
        printf("error while creating thread 1\n");
    }

    err = pthread_create(&tid2, NULL, thread_func2, NULL);
    if(err)
    {
        printf("error while creating thread 2\n");
    }

    pthread_join(tid1, NULL);
    //pthread_join(tid2, NULL);

    return 0;
}
