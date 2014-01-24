#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>

static void sig_hup(int signo)
{
    printf("SIGHUP received, pid = %d\n", getpid());
}

static void pr_ids(char *name)
{
    printf("%s: pid = %d, ppid = %d, pgrp = %d, sid = %d\n", name, getpid(),
            getppid(), getpgrp(), getsid(0));
}

int main(void)
{
    char c;
    pid_t pid;

    pr_ids("parent");
    if((pid = fork()) < 0)
    {
        printf("fork error\n");
    }
    else if(pid > 0)
    {
        sleep(5);
        exit(0);
    }
    else
    {
        pr_ids("child");
        setsid();
        pr_ids("child");
        if (open("/dev/tty", O_RDWR) < 0) {  //验证子进程是否具有控制终端
            printf("open error\n");
            exit(1);
        }
        exit(0);
    }
}

