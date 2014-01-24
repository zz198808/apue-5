#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int i;
    extern char **environ;
    char **ptr;

    for(i = 0; i < argc; i++)
    {
        printf("argv[%d]:%s\n", i, argv[i]);
    }

    for(ptr = environ; *ptr != NULL; ptr++)
    {
        printf("%s\n", *ptr);
    }

    return 0;
}
