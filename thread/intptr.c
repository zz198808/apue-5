#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    int p[10] = {1,2,3,4,5,6,7,8,9};
    int i;
    int *ptr = p;
    intptr_t intptr = (intptr_t)p;

    for(i =  0; i < 10; i++)
    {
        printf("iter:%d, pointer:%p\n", p[i], &p[i]);
    }

    i = 0;
    
    for(; i < 10; ptr++, i++)
    {
        printf("ptr:%d, pointer:%p\n", *ptr, ptr);
    }

    i = 0;
    for(; i < 10; intptr++, i++)
    {
        printf("intptr:%d, pointer:%p\n", *((int *)intptr), intptr);
    }

    return 0;
}
