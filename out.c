#include <stdio.h>

static int test(int val)
{
    int *ptr;

    if(val == 0)
    {
        int val = 4;
        ptr = &val;
    }
    if(val == 0)
    {
        int test = 3;
    }    
    return (*ptr + 1);
}

int main(void)
{
    int i = test(0);
    printf("%d\n", i);
    return 0;
}
