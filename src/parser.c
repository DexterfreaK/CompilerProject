#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include<vectorofvector.h>

/**
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

/**
 * @brief Initializes the grammar for the parser.
 *
 * @return Pointer to the initialized VectorOfVector representing the grammar.
 */
VectorOfVector *initialize_grammar() {
    VectorOfVector *grammar = (VectorOfVector *)malloc(sizeof(VectorOfVector));
    if (!grammar) {
        fprintf(stderr, "Memory allocation error in initialize_grammar\n");
        exit(EXIT_FAILURE);
    }
    initVectorOfVector(grammar);

    // Production 0: <program> ===> <otherFunctions> <mainFunction>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 1);
        pushBack(&prod, 2);
        pushBack(&prod, 3);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 1: <mainFunction> ===> TK_MAIN <stmts> TK_END
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 3);
        pushBack(&prod, 69);
        pushBack(&prod, 4);
        pushBack(&prod, 62);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 2: <otherFunctions> ===> <function> <otherFunctions>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 2);
        pushBack(&prod, 5);
        pushBack(&prod, 2);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 3: <otherFunctions> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 2);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 4: <function> ===> TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 5);
        pushBack(&prod, 58);
        pushBack(&prod, 6);
        pushBack(&prod, 7);
        pushBack(&prod, 80);
        pushBack(&prod, 4);
        pushBack(&prod, 62);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 5: <input_par> ===> TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 6);
        pushBack(&prod, 75);
        pushBack(&prod, 71);
        pushBack(&prod, 72);
        pushBack(&prod, 73);
        pushBack(&prod, 8);
        pushBack(&prod, 74);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 6: <output_par> ===> TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 7);
        pushBack(&prod, 76);
        pushBack(&prod, 71);
        pushBack(&prod, 72);
        pushBack(&prod, 73);
        pushBack(&prod, 8);
        pushBack(&prod, 74);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 7: <output_par> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 7);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 8: <parameter_list> ===> <dataType> TK_ID <remaining_list>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 8);
        pushBack(&prod, 9);
        pushBack(&prod, 55);
        pushBack(&prod, 31);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 9: <dataType> ===> <primitiveDatatype>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 9);
        pushBack(&prod, 10);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 10: <dataType> ===> <constructedDatatype>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 9);
        pushBack(&prod, 11);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 11: <primitiveDatatype> ===> TK_INT
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 10);
        pushBack(&prod, 77);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 12: <primitiveDatatype> ===> TK_REAL
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 10);
        pushBack(&prod, 78);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 13: <constructedDatatype> ===> <A> TK_RUID
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 11);
        pushBack(&prod, 12);
        pushBack(&prod, 59);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 14: <constructedDatatype> ===> TK_RUID
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 11);
        pushBack(&prod, 59);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 15: <remaining_list> ===> TK_COMMA <parameter_list>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 31);
        pushBack(&prod, 79);
        pushBack(&prod, 8);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 16: <remaining_list> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 31);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 17: <stmts> ===> <typeDefinitions> <declarations> <otherStmts> <returnStmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 4);
        pushBack(&prod, 13);
        pushBack(&prod, 14);
        pushBack(&prod, 15);
        pushBack(&prod, 16);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 18: <typeDefinitions> ===> <typeDefinition> <typedefinitions>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 13);
        pushBack(&prod, 17);
        pushBack(&prod, 13);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 19: <typeDefinitions> ===> <definetypestmt> <typeDefinitions>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 13);
        pushBack(&prod, 18);
        pushBack(&prod, 13);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 20: <typeDefinitions> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 13);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 21: <typeDefinition> ===> TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 17);
        pushBack(&prod, 97);
        pushBack(&prod, 59);
        pushBack(&prod, 19);
        pushBack(&prod, 98);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 22: <typeDefinition> ===> TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 17);
        pushBack(&prod, 64);
        pushBack(&prod, 59);
        pushBack(&prod, 19);
        pushBack(&prod, 65);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 23: <fieldDefinitions> ===> <fieldDefinition> <fieldDefinition> <moreFields>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 19);
        pushBack(&prod, 20);
        pushBack(&prod, 20);
        pushBack(&prod, 21);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 24: <fieldDefinition> ===> TK_TYPE <datatype> TK_COLON TK_FIELDID TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 20);
        pushBack(&prod, 68);
        pushBack(&prod, 9);
        pushBack(&prod, 81);
        pushBack(&prod, 54);
        pushBack(&prod, 80);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 25: <moreFields> ===> <fieldDefinition> <moreFields>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 21);
        pushBack(&prod, 20);
        pushBack(&prod, 21);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 26: <moreFields> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 21);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 27: <declarations> ===> <declaration> <declarations>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 14);
        pushBack(&prod, 47);
        pushBack(&prod, 14);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 28: <declarations> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 14);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 29: <declaration> ===> TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 47);
        pushBack(&prod, 68);
        pushBack(&prod, 9);
        pushBack(&prod, 81);
        pushBack(&prod, 55);
        pushBack(&prod, 22);
        pushBack(&prod, 80);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 30: <global_or_not> ===> TK_COLON TK_GLOBAL
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 22);
        pushBack(&prod, 81);
        pushBack(&prod, 70);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 31: <global_or_not> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 22);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 32: <otherStmts> ===> <stmt> <otherStmts>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 15);
        pushBack(&prod, 23);
        pushBack(&prod, 15);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 33: <otherStmts> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 15);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 34: <stmt> ===> <assignmentStmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 23);
        pushBack(&prod, 24);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 35: <stmt> ===> <iterativeStmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 23);
        pushBack(&prod, 25);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 36: <stmt> ===> <conditionalStmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 23);
        pushBack(&prod, 26);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 37: <stmt> ===> <ioStmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 23);
        pushBack(&prod, 27);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 38: <stmt> ===> <funCallStmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 23);
        pushBack(&prod, 28);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 39: <assignmentStmt> ===> <SingleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 24);
        pushBack(&prod, 29);
        pushBack(&prod, 109);
        pushBack(&prod, 30);
        pushBack(&prod, 80);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 40: <singleOrRecId> ===> TK_ID <option_single_constructed>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 29);
        pushBack(&prod, 55);
        pushBack(&prod, 32);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 41: <option_single_constructed> ===> <oneExpansion> <moreExpansions>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 32);
        pushBack(&prod, 33);
        pushBack(&prod, 34);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 42: <option_single_constructed> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 32);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 43: <moreExpansions> ===> <oneExpansion> <moreExpansions>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 34);
        pushBack(&prod, 33);
        pushBack(&prod, 34);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 44: <moreExpansions> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 34);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 45: <oneExpansion> ===> TK_DOT TK_FIELDID
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 33);
        pushBack(&prod, 82);
        pushBack(&prod, 54);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 46: <funCallStmt> ===> <outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 28);
        pushBack(&prod, 35);
        pushBack(&prod, 96);
        pushBack(&prod, 58);
        pushBack(&prod, 60);
        pushBack(&prod, 61);
        pushBack(&prod, 36);
        pushBack(&prod, 80);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 47: <outputParameters> ===> TK_SQL <idList> TK_SQR TK_ASSIGNOP
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 35);
        pushBack(&prod, 73);
        pushBack(&prod, 37);
        pushBack(&prod, 74);
        pushBack(&prod, 109);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 48: <outputParameters> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 35);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 49: <inputParameters> ===> TK_SQL <idList> TK_SQR
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 36);
        pushBack(&prod, 73);
        pushBack(&prod, 37);
        pushBack(&prod, 74);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 50: <iterativeStmt> ===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 25);
        pushBack(&prod, 63);
        pushBack(&prod, 84);
        pushBack(&prod, 38);
        pushBack(&prod, 85);
        pushBack(&prod, 23);
        pushBack(&prod, 15);
        pushBack(&prod, 83);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 51: <conditionalStmt> ===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <factorConditionalStmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 26);
        pushBack(&prod, 86);
        pushBack(&prod, 84);
        pushBack(&prod, 38);
        pushBack(&prod, 85);
        pushBack(&prod, 87);
        pushBack(&prod, 23);
        pushBack(&prod, 15);
        pushBack(&prod, 39);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 52: <factorConditionalStmt> ===> TK_ELSE <stmt> <otherStmts> TK_ENDIF
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 39);
        pushBack(&prod, 99);
        pushBack(&prod, 23);
        pushBack(&prod, 15);
        pushBack(&prod, 88);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 53: <factorConditionalStmt> ===> TK_ELSE <stmt> <otherStmts> TK_ENDIF
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 39);
        pushBack(&prod, 88);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 54: <ioStmt> ===> TK_READ TK_OP <var> TK_CL TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 27);
        pushBack(&prod, 89);
        pushBack(&prod, 84);
        pushBack(&prod, 40);
        pushBack(&prod, 85);
        pushBack(&prod, 80);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 55: <ioStmt> ===> TK_WRITE TK_OP <var> TK_CL TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 27);
        pushBack(&prod, 90);
        pushBack(&prod, 84);
        pushBack(&prod, 40);
        pushBack(&prod, 85);
        pushBack(&prod, 80);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 56: <arithmeticExpression> ===> <arith_term> <arithmatic_recursion>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 30);
        pushBack(&prod, 41);
        pushBack(&prod, 42);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 57: <arithmatic_recursion> ===> <operator> <arith_term> <arithmatic_recursion>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 42);
        pushBack(&prod, 43);
        pushBack(&prod, 41);
        pushBack(&prod, 42);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 58: <arithmatic_recursion> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 42);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 59: <arith_term> ===> <factor> <term_recursion>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 41);
        pushBack(&prod, 44);
        pushBack(&prod, 45);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 60: <term_recursion> ===> <operator_second> <factor> <term_recursion>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 45);
        pushBack(&prod, 46);
        pushBack(&prod, 44);
        pushBack(&prod, 45);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 61: <term_recursion> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 45);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 62: <factor> ===> TK_OP <arithmeticExpression> TK_CL
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 44);
        pushBack(&prod, 84);
        pushBack(&prod, 30);
        pushBack(&prod, 85);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 63: <factor> ===> <var>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 44);
        pushBack(&prod, 40);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 64: <operator> ===> TK_PLUS
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 43);
        pushBack(&prod, 92);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 65: <operator> ===> TK_MINUS
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 43);
        pushBack(&prod, 93);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 66: <operator_second> ===> TK_MUL
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 46);
        pushBack(&prod, 94);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 67: <operator_second> ===> TK_DIV
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 46);
        pushBack(&prod, 95);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 68: <booleanExpression> ===> TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 38);
        pushBack(&prod, 84);
        pushBack(&prod, 38);
        pushBack(&prod, 85);
        pushBack(&prod, 48);
        pushBack(&prod, 84);
        pushBack(&prod, 38);
        pushBack(&prod, 85);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 69: <booleanExpression> ===> <var> <relationalOp> <var>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 38);
        pushBack(&prod, 40);
        pushBack(&prod, 49);
        pushBack(&prod, 40);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 70: <booleanExpression> ===> TK_NOT TK_OP <booleanExpression> TK_CL
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 38);
        pushBack(&prod, 102);
        pushBack(&prod, 84);
        pushBack(&prod, 38);
        pushBack(&prod, 85);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 71: <var> ===> TK_NUM (or TK_RNUM or <singleOrRecId>)
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 40);
        pushBack(&prod, 56);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 72: <var> ===> TK_NUM (or TK_RNUM or <singleOrRecId>)
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 40);
        pushBack(&prod, 57);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 73: <var> ===> TK_NUM (or TK_RNUM or <singleOrRecId>)
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 40);
        pushBack(&prod, 29);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 74: <logicalOp> ===> TK_AND
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 48);
        pushBack(&prod, 100);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 75: <logicalOp> ===> TK_OR
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 48);
        pushBack(&prod, 101);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 76: <relationalOp> ===> TK_LT
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 49);
        pushBack(&prod, 103);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 77: <relationalOp> ===> TK_LE
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 49);
        pushBack(&prod, 104);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 78: <relationalOp> ===> TK_EQ
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 49);
        pushBack(&prod, 105);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 79: <relationalOp> ===> TK_GT
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 49);
        pushBack(&prod, 106);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 80: <relationalOp> ===> TK_GE
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 49);
        pushBack(&prod, 107);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 81: <relationalOp> ===> TK_NE
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 49);
        pushBack(&prod, 108);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 82: <returnStmt> ===> TK_RETURN <optionalReturn> TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 16);
        pushBack(&prod, 91);
        pushBack(&prod, 50);
        pushBack(&prod, 80);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 83: <optionalReturn> ===> TK_SQL <idList> TK_SQR
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 50);
        pushBack(&prod, 73);
        pushBack(&prod, 37);
        pushBack(&prod, 74);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 84: <optionalReturn> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 50);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 85: <idList> ===> TK_ID <more_ids>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 37);
        pushBack(&prod, 55);
        pushBack(&prod, 51);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 86: <more_ids> ===> TK_COMMA <idList>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 51);
        pushBack(&prod, 79);
        pushBack(&prod, 37);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 87: <more_ids> ===> eps
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 51);
        pushBack(&prod, 110);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 88: <definetypestmt> ===> TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 18);
        pushBack(&prod, 66);
        pushBack(&prod, 12);
        pushBack(&prod, 59);
        pushBack(&prod, 67);
        pushBack(&prod, 59);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 89: <A> ===> TK_RECORD
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 12);
        pushBack(&prod, 97);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }
    // Production 90: <A> ===> TK_UNION
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 12);
        pushBack(&prod, 64);
        pushBack(&prod, -1);
        pushBackVector(grammar, prod);
    }

    return grammar;
}

// TODO: Remove this main function before submitting the assignment.
// this main function is just for testing the grammar initialization.
int parser_main()
{
    // Initialize the grammar.
    // FILE *fp = fopen("test.txt", "a+");
    VectorOfVector *grammar = initialize_grammar();

    // Print the grammar using the printVectorOfVector function.
    printf("Grammar Productions:\n");
    printVectorOfVector(grammar);

    // Example: Access the element at production index 'i' and element index 'j'.
    int i = 0, j = 1; // Change these indices as needed.
    int element = accessVectorOfVector(grammar, i, j);
    printf("Element at production %d, index %d: %d\n", i, j, element);

    // Free the allocated memory.
    freeVectorOfVector(grammar);
    free(grammar);

    return 0;
}