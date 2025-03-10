#ifndef STACK_H
#define STACK_H
#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

// typedef struct {
//     Vector data;
//     int start;
// } Deque;
// void createDeque(Deque *dq, int capacity);
// void deleteDeque(Deque *dq);
// int pushFront(Deque *dq, int value);
// int pushBackDeque(Deque *dq, int value);
// int popFront(Deque *dq, int *value);
// int popRear(Deque *dq, int *value);
// int first(Deque *dq, int *value);
// int rear(Deque *dq, int *value);
// void dequeprint(const Deque *dq);

// typedef struct {
//     Deque dq;
// } Stack;

// void createStack(Stack *s, int capacity);
// void deleteStack(Stack *s);
// int pushStack(Stack *s, int value);
// int popStack(Stack *s, int *value);
// int topStack(Stack *s, int *value);
// int isStackEmpty(const Stack *s);

// Stack implementation for TreeNode pointers
typedef struct {
    TreeNode **array;    // Array of TreeNode pointers
    int capacity;        // Maximum capacity of the stack
    int top;             // Index of the top element (-1 if empty)
} Stack;

// Stack operations
void createStack(Stack *stack, int capacity);
void deleteStack(Stack *stack);
int isStackFull(const Stack *stack);
int isStackEmpty(const Stack *stack);
void resizeStack(Stack *stack);
int pushStack(Stack *stack, TreeNode *node);
int popStack(Stack *stack, TreeNode **node);
int topStack(Stack *stack, TreeNode **node);
void printStack(const Stack *stack);

// TreeNode helpers
// TreeNode* createTreeNode(NodeType type, const char* data, int value, TreeNode* parent);
// void freeTreeNode(TreeNode* node);

#endif // STACK_H
// #endif  // STACK_H