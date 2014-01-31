#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

struct foo
{
    int a;
    int b;
    int c;
    int d;
}foo;

struct foo foo1 = {1,2,3,5};

void print_foo(const char *s, struct foo *fp)
{
    printf("%s\n", s);
    printf("pointer:%p\n", fp);
    printf("a:%d\n", fp->a);
    printf("b:%d\n", fp->b);
    printf("c:%d\n", fp->c);
    printf("d:%d\n", fp->d);
}

void * thread_func1(void *args)
{
    struct foo f = {1, 2, 3, 4};
    print_foo("thread 1:\n", &f);
    pthread_exit((void *)&f);
}

void *thread_func2(void *args)
{
    printf("thread2's ID is %d\n", pthread_self());
    pthread_exit((void *)1);
}

int main(int argc, char *argv[])
{
    int err;
    pthread_t tid1;
    pthread_t tid2;
    struct foo *f;

    err = pthread_create(&tid1, NULL, thread_func1, NULL);
    if(err)
    {
        printf("error while creating thread\n");
        return 1;
    }

    err = pthread_join(tid1, (void *)(&f));
    if(err)
    {
        printf("error while join thread 1\n");
        return 2;
    }

    err = pthread_create(&tid2, NULL, thread_func2, NULL);
    if(err)
    {
        printf("error while creating thread2\n");
        return 3;
    }
    sleep(1);
    print_foo("Parent:\n", f);

    return 0;
}
