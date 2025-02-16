#ifndef STACK_H
#define STACK_H
#include <stdlib.h>
#include <stdio.h>
#include "vectorofvector.h"

typedef struct {
    Vector data;
    int start;
} Deque;
void createDeque(Deque *dq, int capacity);
void deleteDeque(Deque *dq);
int pushFront(Deque *dq, int value);
int pushBackDeque(Deque *dq, int value);
int popFront(Deque *dq, int *value);
int popRear(Deque *dq, int *value);
int first(Deque *dq, int *value);
int rear(Deque *dq, int *value);
void dequeprint(const Deque *dq);

typedef struct {
    Deque dq;
} Stack;

void createStack(Stack *s, int capacity);
void deleteStack(Stack *s);
int pushStack(Stack *s, int value);
int popStack(Stack *s, int *value);
int topStack(Stack *s, int *value);
int isStackEmpty(const Stack *s);

#endif  // STACK_H