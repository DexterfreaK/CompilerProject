#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "vectorofvector.h"
static void ensure_deque_capacity(Deque *dq) {
    if (dq->data.size < dq->data.capacity)
        return;

    size_t oldCap = dq->data.capacity;
    size_t newCap = oldCap * 2;
    int *newArr = (int *)malloc(newCap * sizeof(int));
    if (newArr == NULL) {
        fprintf(stderr, "Memory allocation failed in ensure_deque_capacity\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < dq->data.size; i++) {
        newArr[i] = dq->data.data[(dq->start + i) % oldCap];
    }
    free(dq->data.data);
    dq->data.data = newArr;
    dq->data.capacity = newCap;
    dq->start = 0;
}

/* ------------------ Deque Functions ------------------ */

void createDeque(Deque *dq, int capacity) {
    initVector(&dq->data);
    // If a larger capacity was requested, reallocate.
    if ((size_t)capacity > dq->data.capacity) {
        int *newArr = (int *)realloc(dq->data.data, capacity * sizeof(int));
        if (newArr == NULL) {
            fprintf(stderr, "Memory allocation failed in deque_init\n");
            exit(EXIT_FAILURE);
        }
        dq->data.data = newArr;
        dq->data.capacity = capacity;
    }
    dq->data.size = 0;
    dq->start = 0;
}

void deleteDeque(Deque *dq) {
    freeVector(&dq->data);
}

int pushFront(Deque *dq, int value) {
    ensure_deque_capacity(dq);
    dq->start = (dq->start + dq->data.capacity - 1) % dq->data.capacity;
    dq->data.data[dq->start] = value;
    dq->data.size++;
    return 1;
}

int pushBackDeque(Deque *dq, int value) {
    ensure_deque_capacity(dq);
    size_t pos = (dq->start + dq->data.size) % dq->data.capacity;
    dq->data.data[pos] = value;
    dq->data.size++;
    return 1;
}

int popFront(Deque *dq, int *value) {
    if (dq->data.size == 0)
        return 0;  // The deque is empty.
    *value = dq->data.data[dq->start];
    dq->start = (dq->start + 1) % dq->data.capacity;
    dq->data.size--;
    return 1;
}

int popRear(Deque *dq, int *value) {
    if (dq->data.size == 0)
        return 0;  // The deque is empty.
    size_t pos = (dq->start + dq->data.size - 1) % dq->data.capacity;
    *value = dq->data.data[pos];
    dq->data.size--;
    return 1;
}

int first(Deque *dq, int *value) {
    if (dq->data.size == 0)
        return 0;
    *value = dq->data.data[dq->start];
    return 1;
}

int rear(Deque *dq, int *value) {
    if (dq->data.size == 0)
        return 0;
    size_t pos = (dq->start + dq->data.size - 1) % dq->data.capacity;
    *value = dq->data.data[pos];
    return 1;
}

void dequeprint(const Deque *dq) {
    for (size_t i = 0; i < dq->data.size; i++) {
        printf("%d ", dq->data.data[(dq->start + i) % dq->data.capacity]);
    }
    printf("\n");
}

/* ------------------ Stack Functions ------------------ */

void createStack(Stack *s, int capacity) {
    createDeque(&s->dq, capacity);
}

void deleteStack(Stack *s) {
    deleteDeque(&s->dq);
}

int pushStack(Stack *s, int value) {
    return pushBackDeque(&s->dq, value);
}

int popStack(Stack *s, int *value) {
    return popRear(&s->dq, value);
}

int topStack(Stack *s, int *value) {
    return rear(&s->dq, value);
}

int isStackEmpty(const Stack *s) {
    return (s->dq.data.size == 0);
}
//TODO: Remove this main function before submitting the assignment.
// this main function is just for testing the stack implementation.
void stack_main() {
    Stack myStack;
    int item;
    int initialCapacity = 4;  // Start with a capacity of 4

    createStack(&myStack, initialCapacity);

    // Push some items onto the stack
    pushStack(&myStack, 100);
    pushStack(&myStack, 200);
    pushStack(&myStack, 300);
    pushStack(&myStack, 400);

    // Peek at the top item
    if (topStack(&myStack, &item))
        printf("Top of stack: %d\n", item);
    popStack(&myStack, &item);
        printf("Popped: %d\n", item);
    // Peek at the top item
    if (topStack(&myStack, &item))
        printf("Top of stack: %d\n", item);
    popStack(&myStack, &item);
        printf("Popped: %d\n", item);
    // Peek at the top item
    if (topStack(&myStack, &item))
        printf("Top of stack: %d\n", item);
    popStack(&myStack, &item);
        printf("Popped: %d\n", item);
    // Peek at the top item
    if (topStack(&myStack, &item))
        printf("Top of stack: %d\n", item);
    popStack(&myStack, &item);
        printf("Popped: %d\n", item);

    deleteStack(&myStack);
    return 0;
}