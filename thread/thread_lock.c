#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

struct foo
{
    int f_count;
    pthread_mutex_t f_lock;
};

struct foo* foo_alloc(void)
{
    struct foo *fp;

    if((fp = (struct foo *)malloc(sizeof(struct foo))) != NULL)
    {
        fp->f_count = 1;
        if(pthread_mutex_init(&fp->f_lock, NULL) != 0)
        {
            free(fp);
            return NULL;
        }
        return fp;
    }
    else
    {
        return NULL;
    }
}

void foo_hold(struct foo *fp)
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}

void foo_release(struct foo *fp)
{
    pthread_mutex_lock(&fp->f_lock);

    if(--fp->f_count == 0)
    {
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    }
    else
    {
        pthread_mutex_unlock(&fp->f_lock);
    }
}

void *thread_func(void *fp)
{
    struct foo * f = (struct foo *)fp;
    printf("thread ID:%u\n", pthread_self());
    foo_hold(f);
    foo_release(f);
    pthread_exit((void *)0);
}

int main(int argc, char *argv[])
{
    #define THREAD_NUMS 10
    pthread_t tid[THREAD_NUMS];
    int i;
    int err;
    void *tret;
    struct foo *fp;

    fp = foo_alloc();
    if(fp == NULL)
    {
        return 1;
    }

    for(i = 0; i < THREAD_NUMS; i++)
    {
        err = pthread_create(&tid[i], NULL, thread_func, (void *)fp);
        if(err)
        {
            printf("error while create threading %d\n", i);
        } 
    }

    for(i = 0; i < THREAD_NUMS; i++)
    {
        err = pthread_join(tid[i], &tret);
        if(err)
        {
            printf("error while joining thread %d\n", i);
        }
        printf("thread %d exit code %d\n", i, (intptr_t)tret);
    }

    return 0;
}


