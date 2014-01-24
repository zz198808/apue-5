#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define PSCMD "ps ao pid,ppid,state,tty,comm"

int main(void)
{
    pid_t pid;

    if((pid = fork()) < 0)
    {
        printf("fork error\n");
    }
    else if(pid == 0)
    {
        exit(0);
    }
    else
    {
        sleep(4);
        system(PSCMD);
        exit(0);
    }
}
