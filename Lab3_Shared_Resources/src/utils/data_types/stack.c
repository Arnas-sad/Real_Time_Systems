#include "utils/data_types/stack.h"


void init(Stack *s) 
{
    s->top = -1;
}

int stackIsFull(Stack *s) 
{
    return s->top == MAX_STACK - 1;
}

int stackIsEmpty(Stack *s) 
{
    return s->top == -1;
}

void push(Stack *s, int value) {
    if (stackIsFull(s)) {
        printf("Stack overflow! Cannot push %d onto the stack.\n", value);
        return;
    }
    s->data[++(s->top)] = value;
    printf("Pushed %d onto the stack.\n", value);
}

int pop(Stack *s) {
    if (stackIsEmpty(s)) {
        printf("Stack underflow! Cannot pop from an empty stack.\n");
        exit(EXIT_FAILURE); 
    }

    printf("Popping %d from the top of the stack\n", s->data[(s->top)]);
    return s->data[(s->top)--];
}

int peek(Stack *s) {
    if (stackIsEmpty(s)) {
        printf("Stack is empty! Cannot peek.\n");
        exit(EXIT_FAILURE);
    }
    return s->data[s->top];
}