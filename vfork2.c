#include <unistd.h>
#include <stdio.h>

static void f1(void);
static void f2(void);

int main(void)
{
    printf("process id:%d\n", getpid());
    f1();
    f2();
    _exit(0);
}

static void f1(void)
{
    pid_t pid;

    if((pid = vfork()) < 0)
    {
        printf("vfork error\n");
    }
}
static void f2(void)
{
    char buf[1000];
    int i;

    for(i = 0;  i < sizeof(buf); i++)
    {
        buf[i] = 0;
    }

    printf("f2:process id:%d\n", getpid());
}
