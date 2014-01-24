#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static jmp_buf env_alrm;

static void sig_alrm(int signo)
{
    longjmp(env_alrm, 1);
}

unsigned int sleep2(unsigned int nsecs)
{
    if(signal(SIGALRM, sig_alrm) == SIG_ERR)
    {
        return (nsecs);
    }

    alarm(nsecs);
    if(setjmp(env_alrm) == 0)
    {
        pause();
    }

    return (alarm(0));
}

int main(void)
{
    while(1)
    {
        sleep2(1);
        printf("test\n");
    }
    return 0;
}
