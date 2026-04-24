#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h> 


#define MAX_STACK 100

typedef struct Stack {
    int data[MAX_STACK];  // Array to store the values
    int top;        // To keep track of the top index
} Stack;



void init(Stack *s);
int stackIsFull(Stack *s);
int stackIsEmpty(Stack *s);
void push(Stack *s, int value);
int pop(Stack *s);
int peek(Stack *s);


#endif  // STACK_H