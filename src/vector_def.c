#include <stdlib.h>
#include <stdio.h>
#include "vectorofvector.h"

/**1
 * @brief Initializes a Vector.
 *
 * @param v Pointer to the Vector to initialize.
 */
void initVector(Vector *v) {
    v->capacity = INITIAL_CAPACITY;
    v->size = 0;
    v->data = (int *)malloc(v->capacity * sizeof(int));
    if (!v->data) {
        fprintf(stderr, "Memory allocation error in initVector\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Adds an element to the end of the Vector.
 *
 * @param v Pointer to the Vector.
 * @param value The integer value to add.
 */
void pushBack(Vector *v, int value) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        int *temp = realloc(v->data, v->capacity * sizeof(int));
        if (!temp) {
            fprintf(stderr, "Memory allocation error in pushBack\n");
            exit(EXIT_FAILURE);
        }
        v->data = temp;
    }
    v->data[v->size++] = value;
}

/**
 * @brief Frees the memory allocated for the Vector.
 *
 * @param v Pointer to the Vector to free.
 */
void freeVector(Vector *v) {
    if (v->data) {
        free(v->data);
    }
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

/**
 * @brief Accesses an element in the Vector with bounds checking.
 *
 * @param v Pointer to the Vector.
 * @param index The index of the element to access.
 * @return The integer value at the specified index.
 */
int accessVector(Vector *v, int index) {
    if (index < 0 || index >= v->size) {
        fprintf(stderr, "Index %d out of bounds for vector of size %d\n", index, v->size);
        exit(EXIT_FAILURE);
    }
    return v->data[index];
}

/**
 * @brief Prints the elements of the Vector.
 *
 * @param v Pointer to the Vector to print.
 */
void printVector(Vector *v) {
    printf("[");
    for (int i = 0; i < v->size; i++) {
        printf("%d", v->data[i]);
        if (i < v->size - 1)
            printf(", ");
    }
    printf("]");
}

/**
 * @brief Initializes a VectorOfVector.
 *
 * @param vv Pointer to the VectorOfVector to initialize.
 */
void initVectorOfVector(VectorOfVector *vv) {
    vv->capacity = INITIAL_CAPACITY;
    vv->size = 0;
    vv->data = (Vector *)malloc(vv->capacity * sizeof(Vector));
    if (!vv->data) {
        fprintf(stderr, "Memory allocation error in initVectorOfVector\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Adds a Vector to the end of the VectorOfVector.
 *
 * @param vv Pointer to the VectorOfVector.
 * @param v The Vector to add.
 */
void pushBackVector(VectorOfVector *vv, Vector v) {
    if (vv->size == vv->capacity) {
        vv->capacity *= 2;
        Vector *temp = realloc(vv->data, vv->capacity * sizeof(Vector));
        if (!temp) {
            fprintf(stderr, "Memory allocation error in pushBackVector\n");
            exit(EXIT_FAILURE);
        }
        vv->data = temp;
    }
    vv->data[vv->size++] = v;
}

/**
 * @brief Frees the memory allocated for the VectorOfVector.
 *
 * @param vv Pointer to the VectorOfVector to free.
 */
void freeVectorOfVector(VectorOfVector *vv) {
    if (vv->data) {
        for (int i = 0; i < vv->size; i++) {
            freeVector(&vv->data[i]);
        }
        free(vv->data);
    }
    vv->data = NULL;
    vv->size = 0;
    vv->capacity = 0;
}

/**
 * @brief Accesses an element in the VectorOfVector.
 *
 * @param vv Pointer to the VectorOfVector.
 * @param i The index of the Vector to access.
 * @param j The index of the element within the Vector to access.
 * @return The integer value at the specified indices.
 */
int accessVectorOfVector(VectorOfVector *vv, int i, int j) {
    if (i < 0 || i >= vv->size) {
        fprintf(stderr, "Vector index %d out of bounds for vector-of-vector of size %d\n", i, vv->size);
        exit(EXIT_FAILURE);
    }
    return accessVector(&(vv->data[i]), j);
}

/**
 * @brief Prints the elements of the VectorOfVector.
 *
 * @param vv Pointer to the VectorOfVector to print.
 */
void printVectorOfVector(VectorOfVector *vv) {
    printf("[\n");
    for (int i = 0; i < vv->size; i++) {
        printf("  ");
        printVector(&(vv->data[i]));
        if (i < vv->size - 1)
            printf(",");
        printf("\n");
    }
    printf("]\n");
}
Vector *array_to_vector(int *arr, int size)
{
    Vector *v = (Vector *)malloc(sizeof(Vector));
    initVector(v);
    for (int i = 0; i < size; i++)
    {
        pushBack(v, arr[i]);
    }
    return v;
}