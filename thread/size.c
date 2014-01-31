#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    void *ptr;
    int size;
    long lsize;

    printf("ptr:%d,int:%d,long:%d\n", sizeof(ptr), sizeof(size), sizeof(lsize));
    return 0;
}
