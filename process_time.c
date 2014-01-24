#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>

static void print_time(clock_t, struct tms *, struct tms *end);
static void do_cmd(char *);
void pr_exit(int);

int main(int argc, char *argv[])
{
    int i;
    setbuf(stdout, NULL);

    for(i = 1; i < argc; i++)
    {
        do_cmd(argv[i]);
    }
    exit(0);
}

static void do_cmd(char *cmd)
{
    struct tms t_start, t_end;
    clock_t start, end;
    int status;

    printf("\ncommand:%s\n", cmd);
    if((start = times(&t_start)) == -1)
    {
        printf("time error\n");
    }
    if((status = system(cmd)) < 0)
    {
        printf("system error\n");
    }

    if((end = times(&t_end)) == -1)
    {
        printf("time error\n");
    }

    print_time(end - start, &t_start, &t_end);
    pr_exit(status);
}

static void print_time(clock_t real, struct tms *t_start, struct tms *t_end)
{
    static long clktck = 0;
    if(clktck == 0)
    {
        if((clktck = sysconf(_SC_CLK_TCK)) <0 )
        {
            printf("sysconf error\n");
        }
    }

    printf("    real:    %7.2f\n", real / (double)clktck);
    printf("    user:    %7.2f\n", (t_end->tms_utime - t_start->tms_utime) / (double)clktck);
    printf("    sys:     %7.2f\n", (t_end->tms_stime - t_start->tms_stime) / (double)clktck);
    printf("    child user:   %7.2f\n", (t_end->tms_cutime - t_start->tms_cutime) / (double)clktck);
    printf("    child sys :   %7.2f\n", (t_end->tms_cstime - t_start->tms_cstime) / (double)clktck);
}

void pr_exit(int status)
{
    if(WIFEXITED(status))
    {
        printf("normal termination,exit status = %d\n", WEXITSTATUS(status));
    }
    else if(WIFSIGNALED(status))
    {
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),
        #ifdef WCOREDUMP
                WCOREDUMP(status)? " (core file generated)":"");
        #else
            "");
        #endif
    }
    else if(WIFSTOPPED(status))
    {
        printf("child stopped, signal number = %d\n", WSTOPSIG(status));
    }
}

