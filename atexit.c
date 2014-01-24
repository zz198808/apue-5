#include <stdio.h>
#include <stdlib.h>

static void exit1(void)
{
    printf("exit 1 here\n");
}

static void exit2(void)
{
    printf("exit 2 here\n");
}

int main(int argc, char *argv[])
{
    if(atexit(exit1) != 0)
    {
        printf("failure register exit1\n");
    }

    if(atexit(exit2) != 0)
    {
        printf("failure register exit2\n");  
    }

    printf("exit main\n");
    return 0;
}



