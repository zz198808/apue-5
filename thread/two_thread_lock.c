#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#define NHASH 29
#define HASH(fp) ((uintptr_t)fp % NHASH)
struct foo * fh[NHASH] = {NULL};

pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo
{
    int f_count;
    pthread_mutex_t f_lock;
    struct foo * f_next;
    int f_id;
};

struct foo * foo_alloc(void)
{
    struct foo *fp;
    int idx;

    if((fp = (struct foo *)malloc(sizeof(struct foo))) != NULL)
    {
        fp->f_count = 1;
        if(pthread_mutex_init(&fp->f_lock, NULL) != 0)
        {
            free(fp);
            return NULL;
        }

        idx = HASH(fp);
        pthread_mutex_lock(&hashlock);
        fp->f_next = fh[idx];
        fh[idx] = fp;
        pthread_mutex_lock(&fp->f_lock);
        pthread_mutex_unlock(&hashlock);
        pthread_mutex_unlock(&fp->f_lock);
        return fp;
    }
    else
    {
        return NULL;
    }
}

void foo_hold(struct foo *fp)
{
    int idx;
    pthread_mutex_lock(&fp->f_lock);
    idx = HASH(fp);
    fp->f_count++;
    printf("on fh[%d]:%d\n", idx, fp->f_count);
    pthread_mutex_unlock(&fp->f_lock);
}

void foo_release(struct foo *fp)
{
    struct foo *head;
    int idx;
    pthread_mutex_lock(&fp->f_lock);
    if(fp->f_count == 1)
    {
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_lock(&hashlock);
        pthread_mutex_lock(&fp->f_lock);
        if(fp->f_count != 1)
        {
            fp->f_count--;
            pthread_mutex_unlock(&fp->f_lock);
            pthread_mutex_unlock(&hashlock);
            return;
        }
        
        idx = HASH(fp);
        head = fh[idx];

        if(head == fp)
        {
            fh[idx] = fp->f_next;
        }
        else
        {
            while(head->f_next != fp)
            {
                head = head->f_next;
            }
            head->f_next = fp->f_next;
        }
        pthread_mutex_unlock(&hashlock);
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    }
    else
    {
        fp->f_count--;
        idx = HASH(fp);
        printf("on fh[%d]:%d\n", idx, fp->f_count);
        pthread_mutex_unlock(&fp->f_lock); 
    }

}

void print_foos(void)
{
    int i;
    int count = 0;
    struct foo *head;
    for(i = 0; i < NHASH; i++)
    {
        count = 0;
        head = fh[i];
        while(head != NULL)
        {
            count++;
            head = head->f_next;
        }
        printf("fh[%d]:%d\n", i, count);
    }
}

void * thread_fun(void *arg)
{
    struct foo *fp = (struct foo *)arg;

    printf("In thread: %u\n", pthread_self());
    foo_hold(fp);
    foo_release(fp);
    pthread_exit((void *)0);
}

int main(int argc, char *argv[])
{
    #define MAX_FOOS 100
    #define MAX_THREADS 100
    struct foo *fps[MAX_FOOS];
    pthread_t tids[MAX_THREADS];
    int i;
    int j;
    int err;
    void *tret;

    print_foos();

    for(i = 0; i < MAX_FOOS; i++)
    {
        fps[i] = foo_alloc();
    }

    print_foos();

    for(i = 0; i < MAX_FOOS; i++)
    {
        for(j = 0; j < MAX_THREADS; j++)
        {
            err = pthread_create(&tids[j], NULL, thread_fun, (void *)fps[i]);
            if(err)
            {
                printf("error while creating thread : %d in fh[%d]\n", j, HASH(fps[i]));
            }
        }

        for(j = 0; j < MAX_THREADS; j++)
        {
            err = pthread_join(tids[j], (void *)&tret);
            if(err)
            {
                printf("error while joining thread : %d in fh[%d]\n", j, HASH(fps[i]));
            }

            printf("thread :%d in fh[%d]\n", j, HASH(fps[i]));
        } 
    }
    return 0;
}
