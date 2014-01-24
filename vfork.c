#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int glob = 6;

int main(void)
{
    int var;
    int i;
    pid_t pid;
    char buf[20];

    var = 88;
    printf("before fork\n");
    if((pid = fork()) < 0)
    {
        printf("vfork error\n");
    }
    else if(pid == 0)
    {
        glob++;
        var++;
        exit(0);
    }

    //sleep(2);
    //printf("pid = %d, glob = %d, var = %d,\n", getpid(), glob,var);
    i = printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
    sprintf(buf, "%d\n", i);
    write(STDOUT_FILENO, buf, strlen(buf));
    exit(0);
}
