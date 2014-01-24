#include <stdio.h>
#include <stdlib.h>

static void print_time(char *);

int main(void)
{
    pid_t pid;
    if((pid = fork()) < 0)
    {
        printf("fork error\n");
        return;
    }
    else if(pid == 0)
    {
        print_time("output from child\n");
    }
    else 
    {
        print_time("output from parent\n");
    }
    exit(0);
}

static void print_time(char *str)
{
    char *ptr;
    int c;
    setbuf(stdout, NULL);
    for (ptr = str; (c = *ptr++) != 0; )
    {
        putc(c, stdout);
    }
}
