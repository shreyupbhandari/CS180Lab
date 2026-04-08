typedef struct 
{
    int *a;
    int capacity;
    int top;
}stack;

void push(stack *s, int value);
void pop(stack *s); //Could use the conventional pop functionality to return the value that was popped, but kept it simple.
void expandStack(stack *s);
void shrinkStack(stack *s);
void deleteStack(stack *s);