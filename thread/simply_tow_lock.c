#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#define NHASH 29 
#define HASH(fp) ((uintptr_t)fp % NHASH)
struct foo *fh[NHASH];

#define MAX_FOOS 100
#define MAX_THREADS 100
struct foo *fps[MAX_FOOS] = {NULL};
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo
{
    int f_count;
    pthread_mutex_t f_lock;
    struct foo * f_next;
    int f_id;
};

struct foo * foo_alloc(int i)
{
    struct foo *fp;
    int idx;
    fp = (struct foo *)malloc(sizeof(struct foo));
    if(fp != NULL)
    {
        if(pthread_mutex_init(&fp->f_lock, NULL) != 0)
        {
            printf("pthread mutex init error");
            free(fp);
            return NULL;
        }
        idx = HASH(fp);
        pthread_mutex_lock(&hashlock);
        fp->f_next = fh[idx];
        fh[idx] = fp;
        
        pthread_mutex_lock(&fp->f_lock);
        pthread_mutex_unlock(&hashlock);
        fp->f_id = i;
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
    int idx;
    struct foo * head;
    
    pthread_mutex_lock(&hashlock);
    if(fp->f_count <= 0)
    {
        return;
    }
    if(--fp->f_count == 0)
    {
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
        printf("release fps[%d]\n", fp->f_id);
        fps[fp->f_id] = NULL;
        free(fp);
        pthread_mutex_unlock(&hashlock);
    }
    else
    {
        idx = HASH(fp);
        printf("on fh[%d]:%d\n", idx, fp->f_count);
        pthread_mutex_unlock(&hashlock);
    }
}

void * thread_func1(void *arg)
{
    struct foo *fp = (struct foo *)arg;
    foo_hold(fp);
    pthread_exit((void *)0);
}

void * thread_func2(void *arg)
{
    struct foo *fp = (struct foo *)arg;
    foo_release(fp);
    pthread_exit((void *)0);
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

int main(int argc, char *argv[])
{
    pthread_t tids[MAX_THREADS];
    int i;
    int j;
    int err;
    void *tret;

    print_foos();

    for(i = 0; i < MAX_FOOS; i++)
    {
        printf("alloc the %d'th fp\n", i);
        fps[i] = foo_alloc(i);
    }

    print_foos();

    for(i = 0; i < MAX_FOOS; i++)
    {
        if(fps[i] == NULL)
        {
            printf("fps[%d] is NULL now\n", i);
            continue;
        }

        for(j = 0; j < MAX_THREADS; j++)
        {
            if(j < 48)
            {
                err = pthread_create(&tids[j], NULL, thread_func1, (void *)fps[i]);
            }
            /*else
            {
                err = pthread_create(&tids[j], NULL, thread_func2, (void *)fps[i]);
            }
            if(err)
            {
                printf("error while creating thread : %d in fh[%d]\n", j, HASH(fps[i]));
            }*/
        }

        /*for(j = 0; j < MAX_THREADS; j++)
        {
            err = pthread_join(tids[j], (void *)&tret);
            if(err)
            {
                printf("error while joining thread : %d in fh[%d]\n", j, HASH(fps[i]));
            }

            printf("thread :%d in fh[%d]\n", j, HASH(fps[i]));
        }*/
    }
    return 0;
}
