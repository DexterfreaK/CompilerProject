/**
 * @file VectorOfVector.h
 * @brief Header file for a vector-of-vector data structure in C.
 *
 * This file contains the definitions and function prototypes for a simple
 * vector structure for integers and a vector-of-vector structure to hold
 * many Vector objects.
 */

#ifndef VECTOROFVECTOR_H
#define VECTOROFVECTOR_H

#include <stdlib.h>
#include <stdio.h>

#define INITIAL_CAPACITY 4

/**
 * @struct Vector
 * @brief A simple vector structure for integers.
 *
 * @var Vector::data
 * Pointer to the array of integers.
 * @var Vector::size
 * The current number of elements in the vector.
 * @var Vector::capacity
 * The current capacity of the vector.
 */
typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} Vector;

/**
 * @brief Initializes a Vector.
 *
 * @param v Pointer to the Vector to initialize.
 */
void initVector(Vector *v);

/**
 * @brief Adds an element to the end of the Vector.
 *
 * @param v Pointer to the Vector.
 * @param value The integer value to add.
 */
void pushBack(Vector *v, int value);

/**
 * @brief Frees the memory allocated for the Vector.
 *
 * @param v Pointer to the Vector to free.
 */
void freeVector(Vector *v);

/**
 * @brief Accesses an element in the Vector with bounds checking.
 *
 * @param v Pointer to the Vector.
 * @param index The index of the element to access.
 * @return The integer value at the specified index.
 */
int accessVector(Vector *v, int index);

/**
 * @brief Prints the elements of the Vector.
 *
 * @param v Pointer to the Vector to print.
 */
void printVector(Vector *v);

/**
 * @struct VectorOfVector
 * @brief A vector-of-vector structure to hold many Vector objects.
 *
 * @var VectorOfVector::data
 * Pointer to the array of Vectors.
 * @var VectorOfVector::size
 * The current number of Vectors in the vector-of-vector.
 * @var VectorOfVector::capacity
 * The current capacity of the vector-of-vector.
 */
typedef struct {
    Vector *data;
    size_t size;
    size_t capacity;
} VectorOfVector;

/**
 * @brief Initializes a VectorOfVector.
 *
 * @param vv Pointer to the VectorOfVector to initialize.
 */
void initVectorOfVector(VectorOfVector *vv);

/**
 * @brief Adds a Vector to the end of the VectorOfVector.
 *
 * @param vv Pointer to the VectorOfVector.
 * @param v The Vector to add.
 */
void pushBackVector(VectorOfVector *vv, Vector v);

/**
 * @brief Frees the memory allocated for the VectorOfVector.
 *
 * @param vv Pointer to the VectorOfVector to free.
 */
void freeVectorOfVector(VectorOfVector *vv);

/**
 * @brief Accesses an element in the VectorOfVector.
 *
 * @param vv Pointer to the VectorOfVector.
 * @param i The index of the Vector to access.
 * @param j The index of the element within the Vector to access.
 * @return The integer value at the specified indices.
 */
int accessVectorOfVector(VectorOfVector *vv, int i, int j);

/**
 * @brief Prints the elements of the VectorOfVector.
 *
 * @param vv Pointer to the VectorOfVector to print.
 */
void printVectorOfVector(VectorOfVector *vv);

#endif // VECTOROFVECTOR_H