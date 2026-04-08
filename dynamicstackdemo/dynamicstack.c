#include <stdio.h>
#include <stdlib.h>
#include "dynamicstack.h"
#define minCapacity 8 //Whatever size the user prompts to define the size of the stack, when shrinking, this limit is implemented.
#define shrinkingThreshold 0.25f //Our goal is to shrink the stack when only 25% of the total capacity is in use.
void push(stack *s, int value)
{
    if (s->top+1==s->capacity) //If the index is equal to the capacity of the stack, call the expandStack function.
    {   
        expandStack(s);
    }
    s->top++;
    s->a[s->top]=value; //Appending to the stack based on where our position is.
    
}

void pop(stack *s)
{   
    if (s->top == -1) //Can't pop if it is the first operation executed after creating a stack.
    {
        printf("Can't pop the stack if there is nothing in it!\n");
        return;
    }
    s->top--;//Pop the top element.
    float proportionOfStackUsed= (float) (s->top+1)/s->capacity;//Current occupancy is denoted by top+1 since top is the index of the stack.
    if (proportionOfStackUsed<shrinkingThreshold && s->capacity>minCapacity) 
    //For the stack to have a shrinking limit, the minCapacity constant ensures that the size doesn't go below 8.
    {
        shrinkStack(s);
    }
   
}

void expandStack(stack *s)
{
    int *temp; /**Setting a temporary pointer helps us ensure that realloc doesn't fail. If the exsiting pointer was set for realloc, 
    it couldn't return a NULL value, meaning our data would be lost.**/
    int newCapacity=s->capacity*2;
    if (NULL==(temp=(int *)realloc(s->a,newCapacity*sizeof(int))))
    {
        fprintf(stderr, "Failed to extend the array from %d to %d elements!\n",s->capacity,newCapacity);
        exit(1);
    }
    s->a=temp;
    free(temp);
    temp=NULL;
    printf("Stack size changed: %d -> %d (%d / %d were used.)\n",s->capacity,newCapacity,s->top+1,s->capacity);
    s->capacity=newCapacity;
}

void deleteStack(stack *s)
{
    free(s->a); //Deleting a stack is as simpple as freeing the memory it is holding.
    s->a=NULL;
}

void shrinkStack(stack *s)
{
    int *temp;
    int newCapacity=s->capacity/2;
    if (NULL==(temp=realloc(s->a,newCapacity*sizeof(int))))
    {
        fprintf(stderr, "Shrinking failed from %d to %d elements!\n",s->capacity,newCapacity);
        exit(1);
    }
    s->a=temp;
    free(temp);
    temp=NULL;
    printf("Stack size changed: %d -> %d (%d / %d were used.)\n",s->capacity,newCapacity,s->top+1,s->capacity);
    s->capacity=newCapacity;
}
