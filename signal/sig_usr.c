#include <sys/signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void sig_usr(int);

int main(void)
{
    if(signal(SIGUSR1, sig_usr) == SIG_ERR)
    {
        printf("error catching SIGUSER1\n");
    }

    if(signal(SIGUSR2, sig_usr) == SIG_ERR)
    {
        printf("error catching SIGUSR2\n");
    }

    while(1)
    {
        pause();
    }
}

static void sig_usr(int signo)
{
    if(signo == SIGUSR1)
    {
        printf("SIGUSER1 is catching\n");
    }
    else if(signo == SIGUSR2)
    {
        printf("SIGUSR2 is catching\n");
    }
}
 
