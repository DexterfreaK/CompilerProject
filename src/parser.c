#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "vectorofvector.h"

grammar G;
First_Follow F;


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


// Function to print Grammar
void print_grammar(grammar G)
{
    for (int i = 0; i < GRAMMAR_SIZE; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (accessVectorOfVector(G->Grammar,i,j) == -1)
            {
                printf("\n");
                break;
            }
            if (j == 0)
            {
                printf("<%s> ===> ", grammarTerms[accessVectorOfVector(G->Grammar,i,j)]);
            }
            else
            {
                if (grammarTerms[accessVectorOfVector(G->Grammar,i,j)][0] == 'T' || accessVectorOfVector(G->Grammar,i,j) == EPSILON)
                    printf("%s ", grammarTerms[accessVectorOfVector(G->Grammar,i,j)]);
                else
                    printf("<%s> ", grammarTerms[accessVectorOfVector(G->Grammar,i,j)]);
            }
        }
    }
}


// Function to print the first set of grammar terms
void print_first_set(First_Follow F, grammar G)
{
    printf("\n");
    for (int i = 0; i < TERMS_SIZE; i++)
    {
        printf("%s ===> ", grammarTerms[i]);
        for (int j = 0; j < 20; j++)
        {
            if (F->firstset[i][j] == 0)
                break;
            else
                printf("%s, ", grammarTerms[F->firstset[i][j]]);
        }
        printf("\n");
    }
}

// FUnction to print the follow set of non terminals
void print_follow_set(First_Follow F, grammar G)
{
    printf("\n");
    for (int i = NONTERMINALS_START; i < NONTERMINALS_END; i++)
    {
        printf("%s ===> ", grammarTerms[i]);
        for (int j = 0; j < 100; j++)
        {
            if (F->followset[i][j] == 0)
                break;
            else if (F->followset[i][j] == -1)
                printf("$, ");
            else
                printf("%s, ", grammarTerms[F->followset[i][j]]);
        }
        printf("\n");
    }
}



// checks if term is already present
int checker(int *temp, int n)
{
    int i = 0, flag = 0;
    while(temp[i] != 0)
    {
        if (temp[i] == n)
        {
            flag = -1;
            break;
        }
        ++i;
    }
    return flag; // if b is in array a
}




void FirstSet(int n, grammar gmr, int buffer[150], First_Follow FF)
{
    int j = 0;
    for (int i = 0; i < 150; i++)
    {
        if (buffer[i] == 0)
        {
            j = i;
            break;
        }
    }
    if (n < NONTERMINALS_START || n==EPSILON)//if n is a terminal
    {
        buffer[j] = n;
    }
    else
    {
        for (int i = 0; i < GRAMMAR_SIZE; i++)
        {
            if (accessVectorOfVector(gmr->Grammar, i, 0) == n)// if left hand side matches with n
            {
                if (accessVectorOfVector(gmr->Grammar, i, 1) <= 56 || accessVectorOfVector(gmr->Grammar, i, 1) == 110)//if right side has a terminal
                {
                    if (!checker(buffer, accessVectorOfVector(gmr->Grammar, i, 1)))// to ensure no duplicate
                    {
                        buffer[j++] = accessVectorOfVector(gmr->Grammar, i, 1);
                    }
                }
                else
                {   
                   
                    int epsn = 0;
                    for (int k = 1; k < 10; k++)
                    {   
                        if (accessVectorOfVector(gmr->Grammar, i, k) == -1)
                            break;
                        epsn = 0;
                        FirstSet(accessVectorOfVector(gmr->Grammar, i, k), gmr, FF->firstset[accessVectorOfVector(gmr->Grammar, i, k)], FF);
                        for (int l = 0; FF->firstset[accessVectorOfVector(gmr->Grammar, i, k)][l] != 0; l++)
                        {
                            if (FF->firstset[accessVectorOfVector(gmr->Grammar, i, k)][l] == EPSILON)
                                epsn = 1;
                            else
                            {
                                if (!checker(buffer, FF->firstset[accessVectorOfVector(gmr->Grammar, i, k)][l]))
                                {
                                    buffer[j++] = FF->firstset[accessVectorOfVector(gmr->Grammar, i, k)][l];
                                }
                            }
                        }
                        
                        if (epsn!=1)
                            break;
                    }
                    if (epsn)
                    {
                        buffer[j++] = EPSILON;
                    }
                }
            }
        }
    }
}


 void FollowSet(int n, grammar gmr, int buffer[150], First_Follow FF, int check[111])
{
    int j = 0, k;
    if (check[n] == 1)
    {
        check[n] = 0;
        return;
    }
    if (n == 1)
    {
        buffer[j++] = -1;
    }
    for (int i = 0; i < GRAMMAR_SIZE; i++)
    {
        for (int p = 1; p < 12; p++)
        {
            if (accessVectorOfVector(gmr->Grammar,i,p) == -1)
                break;
            if (accessVectorOfVector(gmr->Grammar,i,p) == n)
            {
                for (k = p + 1; accessVectorOfVector(gmr->Grammar,i,k) != -1; k++)
                {
                    int epsn = 0;
                    for (int l = 0; FF->firstset[accessVectorOfVector(gmr->Grammar,i,k)][l] != 0; l++)
                    {
                        if (FF->firstset[accessVectorOfVector(gmr->Grammar,i,k)][l] == EPSILON)
                            epsn = 1;
                        else
                        {
                            if (!checker(buffer, FF->firstset[accessVectorOfVector(gmr->Grammar,i,k)][l]))
                            {
                                buffer[j++] = FF->firstset[accessVectorOfVector(gmr->Grammar,i,k)][l];
                            }
                        }
                    }
                    if (!epsn)
                        break;
                }
                if (accessVectorOfVector(gmr->Grammar,i,k) == -1 && accessVectorOfVector(gmr->Grammar,i,0) != n)
                {
                    check[accessVectorOfVector(gmr->Grammar,i,0)] = 1;
                    FollowSet(accessVectorOfVector(gmr->Grammar,i,0), gmr, FF->followset[accessVectorOfVector(gmr->Grammar,i,0)], FF, check);
                    check[accessVectorOfVector(gmr->Grammar,i,0)] = 0;
                    for (int l = 0; FF->followset[accessVectorOfVector(gmr->Grammar,i,0)][l] != 0; l++)
                    {
                        if (!checker(buffer, FF->followset[accessVectorOfVector(gmr->Grammar,i,0)][l]))
                        {
                            buffer[j++] = FF->followset[accessVectorOfVector(gmr->Grammar,i,0)][l];
                        }
                    }
                }
            }
        }
    }
}




// Takes input as G, computes the FIRST and FOLLOW sets
First_Follow ComputeFirstAndFollowSets(grammar G)
{
    First_Follow F = (First_Follow)malloc(sizeof(First_Follow));
    F->firstset = (int **)malloc(TERMS_SIZE * sizeof(int *));
    for (int i = 0; i < TERMS_SIZE; i++)
    {
        F->firstset[i] = (int *)malloc(20 * sizeof(int));
        for (int j = 0; j < 20; j++)
        {
            F->firstset[i][j] = 0;
        }
    }
    for (int i = 1; i < TERMS_SIZE; i++)
    {
        FirstSet(i, G, F->firstset[i], F);
    }
    F->followset = (int **)malloc(TERMS_SIZE * sizeof(int *));

    for (int i = 1; i < NONTERMINALS_START; i++)
    {
        F->followset[i] = NULL;
    }
    

    for (int i = NONTERMINALS_START; i < NONTERMINALS_END; i++)
    {
        F->followset[i] = (int *)malloc(20 * sizeof(int));
        for (int j = 0; j < 20; j++)
        {
            F->followset[i][j] = 0;
        }
    }
    int check[111] = {0};
    int passes = 1;
    while (passes != 6)
    {
        for (int i = NONTERMINALS_START; i < NONTERMINALS_END; i++)
        {
            FollowSet(i, G, F->followset[i], F, check);
        }
        passes++;
    }
    return F;
}


void freeFirstandFollow(First_Follow F)
{
    for (int i = 0; i < TERMS_SIZE; i++)
    {
        free(F->firstset[i]);
    }
    free(F->firstset);
    for (int i = 0; i <= NONTERMINALS; i++)
    {
        free(F->followset[i]);
    }
    free(F->followset);
    free(F);
}


// Frees the memory allocated to the grammar
void freeGrammar(grammar G)
{
    for (int i = 0; i < GRAMMAR_SIZE; i++)
    {
        free(accessVector(G->Grammar,i));
    }
    free(G->Grammar);
    free(G);
}

/**
 * @brief Initializes the grammar for the parser.
 *
 * @return Pointer to the initialized VectorOfVector representing the grammar.
 */
grammar initialize_grammar() {
    VectorOfVector *gr = (VectorOfVector *)malloc(sizeof(VectorOfVector));
    if (!gr) {
        fprintf(stderr, "Memory allocation error in initialize_grammar\n");
        exit(EXIT_FAILURE);
    }
    initVectorOfVector(gr);

    // Production 1: <program> ===> <otherFunctions> <mainFunction>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 57);  // program
        pushBack(&prod, 58);  // otherFunctions
        pushBack(&prod, 59);  // mainFunction
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 2: <mainFunction> ===> TK_MAIN <stmts> TK_END
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 59);  // mainFunction
        pushBack(&prod, 1);   // TK_MAIN
        pushBack(&prod, 68);  // stmts
        pushBack(&prod, 2);   // TK_END
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 3: <otherFunctions> ===> <function> <otherFunctions>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 58);  // otherFunctions
        pushBack(&prod, 60);  // function
        pushBack(&prod, 58);  // otherFunctions
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 4: <otherFunctions> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 58);   // otherFunctions
        pushBack(&prod, 110);  // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 5: <function> ===> TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 60);  // function
        pushBack(&prod, 3);   // TK_FUNID
        pushBack(&prod, 61);  // input_par
        pushBack(&prod, 62);  // output_par
        pushBack(&prod, 4);   // TK_SEM
        pushBack(&prod, 68);  // stmts
        pushBack(&prod, 2);   // TK_END
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 6: <input_par> ===> TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 61);  // input_par
        pushBack(&prod, 5);   // TK_INPUT
        pushBack(&prod, 6);   // TK_PARAMETER
        pushBack(&prod, 7);   // TK_LIST
        pushBack(&prod, 8);   // TK_SQL
        pushBack(&prod, 63);  // parameter_list
        pushBack(&prod, 9);   // TK_SQR
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 7: <output_par> ===> TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 62);  // output_par
        pushBack(&prod, 10);  // TK_OUTPUT
        pushBack(&prod, 6);   // TK_PARAMETER
        pushBack(&prod, 7);   // TK_LIST
        pushBack(&prod, 8);   // TK_SQL
        pushBack(&prod, 63);  // parameter_list
        pushBack(&prod, 9);   // TK_SQR
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 8: <output_par> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 62);  // output_par
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 9: <parameter_list> ===> <dataType> TK_ID <remaining_list>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 63);  // parameter_list
        pushBack(&prod, 64);  // dataType
        pushBack(&prod, 56);  // TK_ID
        pushBack(&prod, 67);  // remaining_list
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 10: <dataType> ===> <primitiveDataType>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 64);  // dataType
        pushBack(&prod, 65);  // primitiveDataType
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 11: <dataType> ===> <constructedDataType>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 64);  // dataType
        pushBack(&prod, 66);  // constructedDataType
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 12: <primitiveDataType> ===> TK_INT
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 65);  // primitiveDataType
        pushBack(&prod, 11);  // TK_INT
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 13: <primitiveDataType> ===> TK_REAL
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 65);  // primitiveDataType
        pushBack(&prod, 12);  // TK_REAL
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 14: <constructedDataType> ===> <A> TK_RUID
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 66);  // constructedDataType
        pushBack(&prod, 109); // A
        pushBack(&prod, 13);  // TK_RUID
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 15: <constructedDataType> ===> TK_RUID
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 66);  // constructedDataType
        pushBack(&prod, 13);  // TK_RUID
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 16: <remaining_list> ===> TK_COMMA <parameter_list>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 67);  // remaining_list
        pushBack(&prod, 14);  // TK_COMMA
        pushBack(&prod, 63);  // parameter_list
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 17: <remaining_list> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 67);  // remaining_list
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 18: <stmts> ===> <typeDefinitions> <declarations> <otherStmts> <returnStmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 68);  // stmts
        pushBack(&prod, 69);  // typeDefinitions
        pushBack(&prod, 76);  // declarations
        pushBack(&prod, 79);  // otherStmts
        pushBack(&prod, 104); // returnStmt
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 19: <typeDefinitions> ===> <actualOrRedefined> <typeDefinitions>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 69);  // typeDefinitions
        pushBack(&prod, 70);  // actualOrRedefined
        pushBack(&prod, 69);  // typeDefinitions
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 20: <typeDefinitions> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 69);  // typeDefinitions
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 21: <actualOrRedefined> ===> <typeDefinition>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 70);  // actualOrRedefined
        pushBack(&prod, 71);  // typeDefinition
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 22: <actualOrRedefined> ===> <definetypestmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 70);  // actualOrRedefined
        pushBack(&prod, 108); // definetypestmt
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 23: <typeDefinition> ===> TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 71);  // typeDefinition
        pushBack(&prod, 15);  // TK_RECORD
        pushBack(&prod, 13);  // TK_RUID
        pushBack(&prod, 72);  // fieldDefinitions
        pushBack(&prod, 16);  // TK_ENDRECORD
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 24: <typeDefinition> ===> TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 71);  // typeDefinition
        pushBack(&prod, 17);  // TK_UNION
        pushBack(&prod, 13);  // TK_RUID
        pushBack(&prod, 72);  // fieldDefinitions
        pushBack(&prod, 18);  // TK_ENDUNION
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 25: <fieldDefinitions> ===> <fieldDefinition> <fieldDefinition> <moreFields>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 72);  // fieldDefinitions
        pushBack(&prod, 73);  // fieldDefinition
        pushBack(&prod, 73);  // fieldDefinition
        pushBack(&prod, 75);  // moreFields
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 26: <fieldDefinition> ===> TK_TYPE <fieldType> TK_COLON TK_FIELDID TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 73);  // fieldDefinition
        pushBack(&prod, 19);  // TK_TYPE
        pushBack(&prod, 74);  // fieldType
        pushBack(&prod, 20);  // TK_COLON
        pushBack(&prod, 21);  // TK_FIELDID
        pushBack(&prod, 4);   // TK_SEM
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 27: <fieldType> ===> <primitiveDataType>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 74);  // fieldType
        pushBack(&prod, 65);  // primitiveDataType
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 28: <fieldType> ===> <constructedDataType>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 74);  // fieldType
        pushBack(&prod, 66);  // constructedDataType
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 29: <moreFields> ===> <fieldDefinition> <moreFields>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 75);  // moreFields
        pushBack(&prod, 73);  // fieldDefinition
        pushBack(&prod, 75);  // moreFields
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 30: <moreFields> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 75);  // moreFields
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 31: <declarations> ===> <declaration> <declarations>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 76);  // declarations
        pushBack(&prod, 77);  // declaration
        pushBack(&prod, 76);  // declarations
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 32: <declarations> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 76);  // declarations
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 33: <declaration> ===> TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 77);  // declaration
        pushBack(&prod, 19);  // TK_TYPE
        pushBack(&prod, 64);  // dataType
        pushBack(&prod, 20);  // TK_COLON
        pushBack(&prod, 56);  // TK_ID
        pushBack(&prod, 78);  // global_or_not
        pushBack(&prod, 4);   // TK_SEM
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 34: <global_or_not> ===> TK_COLON TK_GLOBAL
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 78);  // global_or_not
        pushBack(&prod, 20);  // TK_COLON
        pushBack(&prod, 22);  // TK_GLOBAL
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 35: <global_or_not> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 78);  // global_or_not
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 36: <otherStmts> ===> <stmt> <otherStmts>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 79);  // otherStmts
        pushBack(&prod, 80);  // stmt
        pushBack(&prod, 79);  // otherStmts
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 37: <otherStmts> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 79);  // otherStmts
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 38: <stmt> ===> <assignmentStmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 80);  // stmt
        pushBack(&prod, 81);  // assignmentStmt
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 39: <stmt> ===> <iterativeStmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 80);  // stmt
        pushBack(&prod, 89);  // iterativeStmt
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 40: <stmt> ===> <conditionalStmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 80);  // stmt
        pushBack(&prod, 90);  // conditionalStmt
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 41: <stmt> ===> <ioStmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 80);  // stmt
        pushBack(&prod, 92);  // ioStmt
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 42: <stmt> ===> <funCallStmt>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 80);  // stmt
        pushBack(&prod, 86);  // funCallStmt
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 43: <assignmentStmt> ===> <SingleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 81);  // assignmentStmt
        pushBack(&prod, 82);  // SingleOrRecId
        pushBack(&prod, 23);  // TK_ASSIGNOP
        pushBack(&prod, 93);  // arithmeticExpression
        pushBack(&prod, 4);   // TK_SEM
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 44: <SingleOrRecId> ===> TK_ID <option_single_constructed>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 82);  // SingleOrRecId
        pushBack(&prod, 56);  // TK_ID
        pushBack(&prod, 83);  // option_single_constructed
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 45: <option_single_constructed> ===> <oneExpansion> <moreExpansions>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 83);  // option_single_constructed
        pushBack(&prod, 84);  // oneExpansion
        pushBack(&prod, 85);  // moreExpansions
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 46: <option_single_constructed> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 83);  // option_single_constructed
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 47: <moreExpansions> ===> <oneExpansion> <moreExpansions>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 85);  // moreExpansions
        pushBack(&prod, 84);  // oneExpansion
        pushBack(&prod, 85);  // moreExpansions
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 48: <moreExpansions> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 85);  // moreExpansions
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 49: <oneExpansion> ===> TK_DOT TK_FIELDID
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 84);  // oneExpansion
        pushBack(&prod, 49);  // TK_DOT
        pushBack(&prod, 21);  // TK_FIELDID
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 50: <funCallStmt> ===> <outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 86);  // funCallStmt
        pushBack(&prod, 87);  // outputParameters
        pushBack(&prod, 50);  // TK_CALL
        pushBack(&prod, 3);   // TK_FUNID
        pushBack(&prod, 51);  // TK_WITH
        pushBack(&prod, 52);  // TK_PARAMETERS
        pushBack(&prod, 88);  // inputParameters
        pushBack(&prod, 4);   // TK_SEM
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 51: <outputParameters> ===> TK_SQL <idList> TK_SQR TK_ASSIGNOP
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 87);  // outputParameters
        pushBack(&prod, 8);   // TK_SQL
        pushBack(&prod, 106); // idList
        pushBack(&prod, 9);   // TK_SQR
        pushBack(&prod, 23);  // TK_ASSIGNOP
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 52: <outputParameters> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 87);  // outputParameters
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 53: <inputParameters> ===> TK_SQL <idList> TK_SQR
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 88);  // inputParameters
        pushBack(&prod, 8);   // TK_SQL
        pushBack(&prod, 106); // idList
        pushBack(&prod, 9);   // TK_SQR
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 54: <iterativeStmt> ===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 89);  // iterativeStmt
        pushBack(&prod, 24);  // TK_WHILE
        pushBack(&prod, 25);  // TK_OP
        pushBack(&prod, 100); // booleanExpression
        pushBack(&prod, 26);  // TK_CL
        pushBack(&prod, 80);  // stmt
        pushBack(&prod, 79);  // otherStmts
        pushBack(&prod, 55);  // TK_ENDWHILE
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 55: <conditionalStmt> ===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 90);  // conditionalStmt
        pushBack(&prod, 27);  // TK_IF
        pushBack(&prod, 25);  // TK_OP
        pushBack(&prod, 100); // booleanExpression
        pushBack(&prod, 26);  // TK_CL
        pushBack(&prod, 28);  // TK_THEN
        pushBack(&prod, 80);  // stmt
        pushBack(&prod, 79);  // otherStmts
        pushBack(&prod, 91);  // elsePart
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 56: <elsePart> ===> TK_ELSE <stmt> <otherStmts> TK_ENDIF
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 91);  // elsePart
        pushBack(&prod, 29);  // TK_ELSE
        pushBack(&prod, 80);  // stmt
        pushBack(&prod, 79);  // otherStmts
        pushBack(&prod, 30);  // TK_ENDIF
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 57: <elsePart> ===> TK_ENDIF
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 91);  // elsePart
        pushBack(&prod, 30);  // TK_ENDIF
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 58: <ioStmt> ===> TK_READ TK_OP <var> TK_CL TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 92);  // ioStmt
        pushBack(&prod, 31);  // TK_READ
        pushBack(&prod, 25);  // TK_OP
        pushBack(&prod, 101); // var
        pushBack(&prod, 26);  // TK_CL
        pushBack(&prod, 4);   // TK_SEM
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 59: <ioStmt> ===> TK_WRITE TK_OP <var> TK_CL TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 92);  // ioStmt
        pushBack(&prod, 32);  // TK_WRITE
        pushBack(&prod, 25);  // TK_OP
        pushBack(&prod, 101); // var
        pushBack(&prod, 26);  // TK_CL
        pushBack(&prod, 4);   // TK_SEM
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 60: <arithmeticExpression> ===> <term> <expPrime>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 93);  // arithmeticExpression
        pushBack(&prod, 95);  // term
        pushBack(&prod, 94);  // expPrime
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 61: <expPrime> ===> <lowPrecedenceOp> <term> <expPrime>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 94);  // expPrime
        pushBack(&prod, 98);  // lowPrecedenceOp
        pushBack(&prod, 95);  // term
        pushBack(&prod, 94);  // expPrime
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 62: <expPrime> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 94);  // expPrime
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 63: <term> ===> <factor> <termPrime>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 95);  // term
        pushBack(&prod, 97);  // factor
        pushBack(&prod, 96);  // termPrime
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 64: <termPrime> ===> <highPrecedenceOp> <factor> <termPrime>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 96);  // termPrime
        pushBack(&prod, 99);  // highPrecedenceOp
        pushBack(&prod, 97);  // factor
        pushBack(&prod, 96);  // termPrime
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 65: <termPrime> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 96);  // termPrime
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 66: <factor> ===> TK_OP <arithmeticExpression> TK_CL
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 97);  // factor
        pushBack(&prod, 25);  // TK_OP
        pushBack(&prod, 93);  // arithmeticExpression
        pushBack(&prod, 26);  // TK_CL
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 67: <factor> ===> <var>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 97);  // factor
        pushBack(&prod, 101); // var
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 68: <lowPrecedenceOp> ===> TK_PLUS
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 98);  // lowPrecedenceOp
        pushBack(&prod, 33);  // TK_PLUS
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 69: <lowPrecedenceOp> ===> TK_MINUS
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 98);  // lowPrecedenceOp
        pushBack(&prod, 34);  // TK_MINUS
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 70: <highPrecedenceOp> ===> TK_MUL
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 99);  // highPrecedenceOp
        pushBack(&prod, 35);  // TK_MUL
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 71: <highPrecedenceOp> ===> TK_DIV
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 99);  // highPrecedenceOp
        pushBack(&prod, 36);  // TK_DIV
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 72: <booleanExpression> ===> TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 100); // booleanExpression
        pushBack(&prod, 25);  // TK_OP
        pushBack(&prod, 100); // booleanExpression
        pushBack(&prod, 26);  // TK_CL
        pushBack(&prod, 102); // logicalOp
        pushBack(&prod, 25);  // TK_OP
        pushBack(&prod, 100); // booleanExpression
        pushBack(&prod, 26);  // TK_CL
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 73: <booleanExpression> ===> <var> <relationalOp> <var>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 100); // booleanExpression
        pushBack(&prod, 101); // var
        pushBack(&prod, 103); // relationalOp
        pushBack(&prod, 101); // var
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 74: <booleanExpression> ===> TK_NOT TK_OP <booleanExpression> TK_CL
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 100); // booleanExpression
        pushBack(&prod, 37);  // TK_NOT
        pushBack(&prod, 25);  // TK_OP
        pushBack(&prod, 100); // booleanExpression
        pushBack(&prod, 26);  // TK_CL
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 75: <var> ===> <SingleOrRecId>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 101); // var
        pushBack(&prod, 82);  // SingleOrRecId
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 76: <var> ===> TK_NUM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 101); // var
        pushBack(&prod, 53);  // TK_NUM
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 77: <var> ===> TK_RNUM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 101); // var
        pushBack(&prod, 54);  // TK_RNUM
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 78: <logicalOp> ===> TK_AND
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 102); // logicalOp
        pushBack(&prod, 38);  // TK_AND
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 79: <logicalOp> ===> TK_OR
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 102); // logicalOp
        pushBack(&prod, 39);  // TK_OR
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 80: <relationalOp> ===> TK_LT
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 103); // relationalOp
        pushBack(&prod, 40);  // TK_LT
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 81: <relationalOp> ===> TK_LE
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 103); // relationalOp
        pushBack(&prod, 41);  // TK_LE
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 82: <relationalOp> ===> TK_EQ
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 103); // relationalOp
        pushBack(&prod, 42);  // TK_EQ
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 83: <relationalOp> ===> TK_GT
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 103); // relationalOp
        pushBack(&prod, 43);  // TK_GT
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 84: <relationalOp> ===> TK_GE
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 103); // relationalOp
        pushBack(&prod, 44);  // TK_GE
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 85: <relationalOp> ===> TK_NE
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 103); // relationalOp
        pushBack(&prod, 45);  // TK_NE
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 86: <returnStmt> ===> TK_RETURN <optionalReturn> TK_SEM
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 104); // returnStmt
        pushBack(&prod, 46);  // TK_RETURN
        pushBack(&prod, 105); // optionalReturn
        pushBack(&prod, 4);   // TK_SEM
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 87: <optionalReturn> ===> TK_SQL <idList> TK_SQR
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 105); // optionalReturn
        pushBack(&prod, 8);   // TK_SQL
        pushBack(&prod, 106); // idList
        pushBack(&prod, 9);   // TK_SQR
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 88: <optionalReturn> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 105); // optionalReturn
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 89: <idList> ===> TK_ID <more_ids>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 106); // idList
        pushBack(&prod, 56);  // TK_ID
        pushBack(&prod, 107); // more_ids
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 90: <more_ids> ===> TK_COMMA <idList>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 107); // more_ids
        pushBack(&prod, 14);  // TK_COMMA
        pushBack(&prod, 106); // idList
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 91: <more_ids> ===> <eps>
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 107); // more_ids
        pushBack(&prod, 110); // eps
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 92: <definetypestmt> ===> TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 108); // definetypestmt
        pushBack(&prod, 47);  // TK_DEFINETYPE
        pushBack(&prod, 109); // A
        pushBack(&prod, 13);  // TK_RUID
        pushBack(&prod, 48);  // TK_AS
        pushBack(&prod, 13);  // TK_RUID
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 93: <A> ===> TK_RECORD
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 109); // A
        pushBack(&prod, 15);  // TK_RECORD
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }
    // Production 94: <A> ===> TK_UNION
    {
        Vector prod;
        initVector(&prod);
        pushBack(&prod, 109); // A
        pushBack(&prod, 17);  // TK_UNION
        pushBack(&prod, -1);
        pushBackVector(gr, prod);
    }

    grammar G = (grammar)malloc(sizeof(grammar));
    G->Grammar = (VectorOfVector *)malloc(sizeof(VectorOfVector));
    initVectorOfVector(G->Grammar);

    for (int i = 0; i < GRAMMAR_SIZE; i++) {
        Vector v;
        initVector(&v);
    
    for (int j = 0; j < 10; j++) {
        pushBack(&v, accessVectorOfVector(gr,i,j));
        if (accessVectorOfVector(gr,i,j)== -1)
            break;
    }
    
    pushBackVector(G->Grammar, v);
}

return G;

}


// TODO: Remove this main function before submitting the assignment.
// this main function is just for testing the grammar initialization.
int main() {
    // char *testFile = "testcase.txt";
    // char *outputFile = "output.txt";
    
    printf("Initializing grammar...\n");
    grammar G = initialize_grammar();
    // print_grammar(G);
    
    printf("Computing FIRST and FOLLOW sets...\n");
    First_Follow F = ComputeFirstAndFollowSets(G);
    // printf("hello this is testing of first");
    // print_first_set(F, G);
    printf("hello this is testing of follow");
    print_follow_set(F, G);
    
    // printf("Initializing parse table...\n");
    // table T = initialize_table(F);
    // createParseTable(F, T);
    // printTable(T);
    
    // printf("Parsing input source code...\n");
    // parseTree root = parseInputSourceCode(testFile, T);
    // printParseTree(root, outputFile);
    
    // printf("Testing stack operations...\n");
    // Stack *s = newStack();
    // parseTree node = createNewNode();
    // push(s, node);
    // print_stack(s);
    // pop(s);
    // printf("Stack empty: %d\n", isEmpty(s));
    
    printf("Freeing allocated memory...\n");
    // freeParseTree(root);
    freeFirstandFollow(F);
    // freeTable(T);
    freeGrammar(G);
    // freeDataStructures();
    
    printf("Testing complete!\n");
    return 0;
}