#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid;

    if((pid = fork()) < 0)
    {
        printf("fork error\n");
    }
    else if(pid == 0)
    {
        if(execlp("echoall", "echoall", "apue", "execlp", "test", (char *)0) < 0)
        {
            printf("execlp error\n");
        }
    }

    if(waitpid(pid, NULL, 0) < 0)
    {
        printf("wait error\n");
    }

    if((pid = fork()) < 0)
    {
        printf("fork error\n");
    }
    else if(pid == 0)
    {
        char *envi[] = {"execve", "test", NULL};
        char *ar[] = {"echoall", "apue", "execve", "test", NULL};
        if(execve("/vagrant/echoall", ar, envi) < 0)
        {
            printf("execve error\n");
        }
    }
    if(waitpid(pid, NULL, 0) < 0)
    {
        printf("wait error\n");
    }
    exit(0);
}
