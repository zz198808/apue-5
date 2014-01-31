#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

struct foo
{
    int data;
    struct foo *next;
};

struct foo *head = NULL;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;

void * thread_fun1(void *arg)
{
    struct foo *p;
    while(1)
    {
        pthread_mutex_lock(&mux);
        while(head == NULL)
        {
            pthread_cond_wait(&cond, &mux);
        }

        p = head;
        printf("data:%d\n", p->data);
        head = head->next;
        free(p);
        pthread_mutex_unlock(&mux);
    }
}

void * thread_fun2(void *arg)
{
    #define MAX_DATA 100
    int i;
    struct foo *p = NULL;

    for(i = 0; i < MAX_DATA; i++)
    {
        p = (struct foo *)malloc(sizeof(struct foo));
        p->data = i;
        pthread_mutex_lock(&mux);
        p->next = head;
        head = p;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mux);
        sleep(1);
    }
    printf("test\n");

    pthread_exit((void *)0);
}

int main(int argc, char *argv[])
{
    pthread_t tid1, tid2;
    int err;
    void *tret;

    err = pthread_create(&tid1, NULL, thread_fun1, NULL);
    if(err)
    {
        printf("error while creating thread 1\n");
    }

    err = pthread_create(&tid2, NULL, thread_fun2, NULL);
    if(err)
    {
        printf("error while creating thread 2\n");
    }

    pthread_join(tid2, &tret);
    printf("thread 2 exit code:%d\n", (intptr_t)tret);
    printf("successfully join thread 2\n");
    pthread_cancel(tid1);
    printf("send a end signal to thread 1\n");
    pthread_join(tid1, NULL);
    printf("successfully join thread 1\n");
    return 0;
}
