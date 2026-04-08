#include <stdio.h>
#include <stdlib.h>
#include "dynamicstack.h"

stack createStack(int capacity);

int main()
{   
    stack s=createStack(8); //User creates a stack of size 8 (8*4)=32bytes to be precise.
    printf("Starting...\n");

    const int max = 64;
    for (int i = 0; i < max; i++) {
        push(&s, i);
    }

    for (int i = 0; i < max; i++) {
        pop(&s);
    }
    deleteStack(&s);
    printf("...done.\n");
    return 0;
}

stack createStack(int capacity)
{
    stack s;
    if (NULL==(s.a=(int *)malloc(sizeof(int)*capacity)))
    {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    s.capacity=capacity;
    s.top=-1; //Top indicates the number of elements in the stack at present. Since there is none on initializaiton, the value is set to -1.

    return s;
}

