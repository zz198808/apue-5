#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_t ntid;

void print_ids(const char *s)
{
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid, (unsigned int)tid, (unsigned int) tid);
}

void *thread_fun(void *args)
{
    print_ids("New Thread:");
    return ((void *) 0);
}

int main(void)
{
    int err;

    err = pthread_create(&ntid, NULL, thread_fun, NULL);
    if(err != 0)
    {
        printf("Pthread Create Error\n");
    }

    print_ids("Main Thread:");
    sleep(1);
    exit(0);
}
