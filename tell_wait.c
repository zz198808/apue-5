#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

static int pipe_file1[2], pipe_file2[2];

void TELL_WAIT(void)
{
    if(pipe(pipe_file1) == -1 || pipe(pipe_file2) == -1)
    {
        printf("pipe file created error\n");
    }
}

void WAIT_PARENT(void)
{
    char c;
    if(read(pipe_file1[0], &c, 1) != 1)
    {
        printf("error\n");
        printf("rece from parent:%c\n", c);
    }
    else if(c != 'p')
    {
        printf("wrong chacter from parent\n");
    }
}

void TELL_CHILD(void)
{
    if(write(pipe_file1[1], "p", 1) == -1)
    {
        printf("error while write pipe_file1[1]\n");
    }
    else
    {
        ;
    }
}

int main(void)
{
    pid_t pid;
    TELL_WAIT();

    if((pid = fork()) < 0)
    {
        printf("fork error\n");
        return 1;
    }
    else if(pid == 0)
    {
        WAIT_PARENT();
        printf("after parent\n");
        _exit(0);
    }
    else
    {
        printf("before child\n");
        TELL_CHILD();
        _exit(0);
    }
}
