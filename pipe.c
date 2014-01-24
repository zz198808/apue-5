#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define MSG_SIZE 100

int main(void)
{
    int pipe_files[2];
    pid_t pid;
    char msg[MSG_SIZE];


    if(pipe(pipe_files) == -1)
    {
        printf("open pipe files error\n");
        return 1;
    }

    if((pid = fork()) < 0)
    {
        printf("fork error\n");
        return -1;
    }
    else if(pid == 0) // child process
    {
        close(pipe_files[1]);
        if(read(pipe_files[0], msg, MSG_SIZE) != -1)
        {
            printf("read from parent %s\n", msg);
        }
        else
        {
            printf("error\n");
            return 1;
        }
        _exit(0);
    }
    else
    {
        close(pipe_files[0]);
        char msg[] = "write the line from parent";
        if(write(pipe_files[1], msg, sizeof(msg)) != -1)
        {
            printf("write successfully\n");
        }
        else
        {
            printf("write error\n");
            return 1;
        }
        _exit(0);
    }

}
