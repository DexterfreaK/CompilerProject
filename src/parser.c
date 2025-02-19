#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "vectorofvector.h"

grammar G;
First_Follow F;
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


// Function to print the first set of grammar terms using VectorOfVector
// void print_first_set(First_Follow F, grammar G)
// {
//     FILE *fp=fopen("first_out.txt","w");
//     fprintf(fp,"\n");
//     printf("\n");
//     for (int i = 0; i < TERMS_SIZE; i++)
//     {
//         fprintf(fp,"%s ===> ", grammarTerms[i]);
//         printf("%s ===> ", grammarTerms[i]);
//         // Retrieve the vector holding the first set for term i.
//         Vector *firstVec = &F->firstset.data[i];
//         for (int j = 0; j < firstVec->size; j++)
//         {
//             // If using a 0 as a sentinel, break early.
//             if (firstVec->data[j] == 0)
//             break;
//             else
//                 fprintf(fp,"%s, ", grammarTerms[firstVec->data[j]]);
//                 printf("%s, ", grammarTerms[firstVec->data[j]]);
//         }
//         printf("\n");
//         fprintf("\n");
//     }
// }
void print_first_set(First_Follow F, grammar G)
{
    FILE *fp = fopen("first_out.txt", "w");  // Or specify a path if needed
    if (fp == NULL) {
        perror("Failed to open file");
        return; // or handle the error as needed
    }
    
    fprintf(fp, "\n");  // Correct order of arguments
    // printf("\n");
    
    for (int i = 0; i < TERMS_SIZE; i++)
    {
        fprintf(fp, "%s ===> ", grammarTerms[i]);
        // printf("%s ===> ", grammarTerms[i]);
        
        // Retrieve the vector holding the first set for term i.
        Vector *firstVec = &F->firstset.data[i];
        for (int j = 0; j < firstVec->size; j++)
        {
            // If using a 0 as a sentinel, break early.
            if (firstVec->data[j] == 0)
                break;
            else {
                fprintf(fp, "%s, ", grammarTerms[firstVec->data[j]]);
                // printf("%s, ", grammarTerms[firstVec->data[j]]);
            }
        }
        // printf("\n");
        fprintf(fp, "\n");
    }
    
    fclose(fp);
}

// // Function to print the follow set of non-terminals using VectorOfVector
// void print_follow_set(First_Follow F, grammar G)
// {
//     // FILE *fp=fopen("follow_output.txt","w+");
//     FILE *fp=fopen("follow_out.txt","w+");
//     if(fp==NULL)
//     printf("error opening file !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//     // fprintf("\n",fp);
//     printf("\n");
//     for (int i = NONTERMINALS_START; i < NONTERMINALS_END; i++)
//     {
//         // fprintf("%s ===> ", grammarTerms[i],fp);
//         printf("%s ===> ", grammarTerms[i]);
//         // Retrieve the vector holding the follow set for non-terminal i.
//         Vector *followVec = &F->followset.data[i];
//         for (int j = 0; j < followVec->size; j++)
//         {
//             // Break on sentinel 0; print '$' if the element is -1.
//             if (followVec->data[j] == 0)
//                 break;
//             else if (followVec->data[j] == -1)
//                 // fprintf("$, ",fp);
//                 printf("$, ");
//             else
//                 // fprintf("%s, ", grammarTerms[followVec->data[j]],fp);
//                 printf("%s, ", grammarTerms[followVec->data[j]]);
//         }
//         // fprintf("\n",fp);
//         printf("\n");
//     }
// }
void print_follow_set(First_Follow F, grammar G)
{
    FILE *fp = fopen("follow_out.txt", "w+");  // Change path if needed
    if(fp == NULL) {
        perror("Error opening file");
        return;
    }

    // Print a newline at the beginning (to both file and terminal)
    fprintf(fp, "\n");
    // printf("\n");

    for (int i = NONTERMINALS_START; i < NONTERMINALS_END; i++)
    {
        // Print the non-terminal header
        fprintf(fp, "%s ===> ", grammarTerms[i]);
        // printf("%s ===> ", grammarTerms[i]);

        // Retrieve the vector holding the follow set for non-terminal i.
        Vector *followVec = &F->followset.data[i];
        for (int j = 0; j < followVec->size; j++)
        {
            // Break on sentinel 0; print '$' if the element is -1.
            if (followVec->data[j] == 0)
                break;
            else if (followVec->data[j] == -1) {
                fprintf(fp, "$, ");
                // printf("$, ");
            }
            else {
                fprintf(fp, "%s, ", grammarTerms[followVec->data[j]]);
                // printf("%s, ", grammarTerms[followVec->data[j]]);
            }
        }
        // End the line for this non-terminal
        fprintf(fp, "\n");
        // printf("\n");
    }

    fclose(fp);  // Always close the file when done
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
    // Find the first available slot in buffer (assuming a 0 sentinel)
    for (int i = 0; i < 150; i++) {
        if (buffer[i] == 0) {
            j = i;
            break;
        }
    }

    // If n is a terminal (or EPSILON), simply add it to the buffer.
    if (n < NONTERMINALS_START || n == EPSILON) {
        buffer[j] = n;
    }
    else {
        // Iterate over all productions in the grammar.
        for (int i = 0; i < GRAMMAR_SIZE; i++) {
            // If the left-hand side matches n.
            if (accessVectorOfVector(gmr->Grammar, i, 0) == n) {
                int rhsSymbol = accessVectorOfVector(gmr->Grammar, i, 1);
                // If the right-hand side is a terminal.
                if (rhsSymbol <= 56 || rhsSymbol == 110) {
                    if (!checker(buffer, rhsSymbol)) {
                        buffer[j++] = rhsSymbol;
                    }
                }
                else {
                    int epsn = 0;
                    // Process the right-hand side symbols one by one.
                    for (int k = 1; k < 10; k++) {
                        int symbol = accessVectorOfVector(gmr->Grammar, i, k);
                        if (symbol == -1)
                            break;
                        epsn = 0;
                        // Recursively compute the first set for the current symbol.
                        // Note: We pass the underlying array of the vector.
                        FirstSet(symbol, gmr, FF->firstset.data[symbol].data, FF);

                        // Now iterate through the first set computed for 'symbol'
                        Vector *firstSetVector = &FF->firstset.data[symbol];
                        for (int l = 0; l < firstSetVector->size; l++) {
                            int curr = firstSetVector->data[l];
                            // If a 0 is encountered, assume it as a sentinel.
                            if (curr == 0)
                                break;
                            if (curr == EPSILON)
                                epsn = 1;
                            else {
                                if (!checker(buffer, curr)) {
                                    buffer[j++] = curr;
                                }
                            }
                        }
                        // If EPSILON was not present in the first set, stop.
                        if (epsn != 1)
                            break;
                    }
                    // If every symbol in the production can derive EPSILON, add EPSILON.
                    if (epsn) {
                        buffer[j++] = EPSILON;
                    }
                }
            }
        }
    }
}



// void FirstSet(int n, grammar gmr, int buffer[150], First_Follow FF)
// {
//     int j = 0;
//     for (int i = 0; i < 150; i++)
//     {
//         if (buffer[i] == 0)
//         {
//             j = i;
//             break;
//         }
//     }
//     if (n < NONTERMINALS_START || n==EPSILON)//if n is a terminal
//     {
//         buffer[j] = n;
//     }
//     else
//     {
//         for (int i = 0; i < GRAMMAR_SIZE; i++)
//         {
//             if (accessVectorOfVector(gmr->Grammar, i, 0) == n)// if left hand side matches with n
//             {
//                 if (accessVectorOfVector(gmr->Grammar, i, 1) <= 56 || accessVectorOfVector(gmr->Grammar, i, 1) == 110)//if right side has a terminal
//                 {
//                     if (!checker(buffer, accessVectorOfVector(gmr->Grammar, i, 1)))// to ensure no duplicate
//                     {
//                         buffer[j++] = accessVectorOfVector(gmr->Grammar, i, 1);
//                     }
//                 }
//                 else
//                 {   
                   
//                     int epsn = 0;
//                     for (int k = 1; k < 10; k++)
//                     {   
//                         if (accessVectorOfVector(gmr->Grammar, i, k) == -1)
//                             break;
//                         epsn = 0;
//                         FirstSet(accessVectorOfVector(gmr->Grammar, i, k), gmr, FF->firstset[accessVectorOfVector(gmr->Grammar, i, k)], FF);
//                         for (int l = 0; FF->firstset[accessVectorOfVector(gmr->Grammar, i, k)][l] != 0; l++)
//                         {
//                             if (FF->firstset[accessVectorOfVector(gmr->Grammar, i, k)][l] == EPSILON)
//                                 epsn = 1;
//                             else
//                             {
//                                 if (!checker(buffer, FF->firstset[accessVectorOfVector(gmr->Grammar, i, k)][l]))
//                                 {
//                                     buffer[j++] = FF->firstset[accessVectorOfVector(gmr->Grammar, i, k)][l];
//                                 }
//                             }
//                         }
                        
//                         if (epsn!=1)
//                             break;
//                     }
//                     if (epsn)
//                     {
//                         buffer[j++] = EPSILON;
//                     }
//                 }
//             }
//         }
//     }
// }


//  void FollowSet(int n, grammar gmr, int buffer[150], First_Follow FF, int check[111])
// {
//     int j = 0, k;
//     if (check[n] == 1)
//     {
//         check[n] = 0;
//         return;
//     }
//     if (n == NONTERMINALS_START) // Assuming first non-terminal is the start symbol
//     {
//         buffer[j++] = -1; // '$' is represented as 0
//     }

//     for (int i = 0; i < GRAMMAR_SIZE; i++)
//     {
//         for (int p = 1; p < 12; p++)
//         {
//             if (accessVectorOfVector(gmr->Grammar,i,p) == -1)
//                 break;
//             if (accessVectorOfVector(gmr->Grammar,i,p) == n)
//             {
//                 for (k = p + 1; accessVectorOfVector(gmr->Grammar,i,k) != -1; k++)
//                 {
//                     int epsn = 0;
//                     for (int l = 0; FF->firstset[accessVectorOfVector(gmr->Grammar,i,k)][l] != 0; l++)
//                     {
//                         if (FF->firstset[accessVectorOfVector(gmr->Grammar,i,k)][l] == EPSILON)
//                             epsn = 1;
//                         else
//                         {
//                             if (!checker(buffer, FF->firstset[accessVectorOfVector(gmr->Grammar,i,k)][l]))
//                             {
//                                 buffer[j++] = FF->firstset[accessVectorOfVector(gmr->Grammar,i,k)][l];
//                             }
//                         }
//                     }
//                     if (!epsn)
//                         break;
//                 }
//                 if (accessVectorOfVector(gmr->Grammar,i,k) == -1 && accessVectorOfVector(gmr->Grammar,i,0) != n)
//                 {
//                     check[accessVectorOfVector(gmr->Grammar,i,0)] = 1;
//                     FollowSet(accessVectorOfVector(gmr->Grammar,i,0), gmr, FF->followset[accessVectorOfVector(gmr->Grammar,i,0)], FF, check);
//                     check[accessVectorOfVector(gmr->Grammar,i,0)] = 0;
//                     for (int l = 0; FF->followset[accessVectorOfVector(gmr->Grammar,i,0)][l] != 0; l++)
//                     {
//                         if (!checker(buffer, FF->followset[accessVectorOfVector(gmr->Grammar,i,0)][l]))
//                         {
//                             buffer[j++] = FF->followset[accessVectorOfVector(gmr->Grammar,i,0)][l];
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }
void FollowSet(int n, grammar gmr, int buffer[150], First_Follow FF, int check[111])
{
    int j = 0, k;
    
    // Prevent infinite recursion by using the check array.
    if (check[n] == 1) {
        check[n] = 0;
        return;
    }
    
    // If n is the start symbol, add the end-of-input marker.
    if (n == NONTERMINALS_START) {
        buffer[j++] = -1; // '$' is represented as -1 here.
    }
    
    // Iterate over all productions in the grammar.
    for (int i = 0; i < GRAMMAR_SIZE; i++) {
        // Iterate over the right-hand side symbols (starting at position 1).
        for (int p = 1; p < 12; p++) {
            int symbolAtP = accessVectorOfVector(gmr->Grammar, i, p);
            if (symbolAtP == -1)
                break;
            // If the current symbol equals n...
            if (symbolAtP == n) {
                // Process the symbols that follow n in the production.
                for (k = p + 1; accessVectorOfVector(gmr->Grammar, i, k) != -1; k++) {
                    int epsn = 0;
                    int followingSymbol = accessVectorOfVector(gmr->Grammar, i, k);
                    // Get the first set vector for the following symbol.
                    Vector *firstVec = &FF->firstset.data[followingSymbol];
                    for (int l = 0; l < firstVec->size; l++) {
                        int curr = firstVec->data[l];
                        if (curr == 0)
                            break;  // Sentinel reached.
                        if (curr == EPSILON)
                            epsn = 1;
                        else {
                            if (!checker(buffer, curr)) {
                                buffer[j++] = curr;
                            }
                        }
                    }
                    // If the current symbol's first set does not contain EPSILON, stop.
                    if (!epsn)
                        break;
                }
                // If we reached the end of the production (or all following symbols can derive EPSILON)
                // and the left-hand side is not n, then add the follow set of the left-hand side.
                if (accessVectorOfVector(gmr->Grammar, i, k) == -1 &&
                    accessVectorOfVector(gmr->Grammar, i, 0) != n)
                {
                    int lhs = accessVectorOfVector(gmr->Grammar, i, 0);
                    check[lhs] = 1;
                    // Recursively compute the follow set for the left-hand side.
                    FollowSet(lhs, gmr, FF->followset.data[lhs].data, FF, check);
                    check[lhs] = 0;
                    // Get the follow set vector for the left-hand side.
                    Vector *followVec = &FF->followset.data[lhs];
                    for (int l = 0; l < followVec->size; l++) {
                        int curr = followVec->data[l];
                        if (curr == 0)
                            break;  // Sentinel reached.
                        if (!checker(buffer, curr)) {
                            buffer[j++] = curr;
                        }
                    }
                }
            }
        }
    }
}


First_Follow ComputeFirstAndFollowSets(grammar G)
{
    // Allocate the First_Follow structure.
    First_Follow F = (First_Follow)malloc(sizeof(First_Follow));

    // Initialize firstset as a VectorOfVector.
    initVectorOfVector(&F->firstset);
    // Create a vector for each index (0 .. TERMS_SIZE-1) and fill it with 20 zeros.
    for (int i = 0; i < TERMS_SIZE; i++) {
        Vector row;
        initVector(&row);
        for (int j = 0; j < 20; j++) {
            pushBack(&row, 0);
        }
        pushBackVector(&F->firstset, row);
    }

    // Compute the First sets.
    // (Assuming FirstSet now expects a pointer to an int array; we pass the underlying array.)
    for (int i = 1; i < TERMS_SIZE; i++) {
        FirstSet(i, G, F->firstset.data[i].data, F);
    }

    // Initialize followset as a VectorOfVector.
    initVectorOfVector(&F->followset);
    /* 
       The original code set followset[1..NONTERMINALS_START-1] = NULL.
       Here we create “dummy” vectors for those indices.
    */
    for (int i = 0; i < NONTERMINALS_START; i++) {
        Vector dummy;
        initVector(&dummy);
        pushBackVector(&F->followset, dummy);
    }
    // For the non-terminals, allocate a vector of 20 zeros.
    for (int i = NONTERMINALS_START; i < NONTERMINALS_END; i++) {
        Vector row;
        initVector(&row);
        for (int j = 0; j < 20; j++) {
            pushBack(&row, 0);
        }
        pushBackVector(&F->followset, row);
    }

    int check[111] = {0};
    int passes = 1;
    while (passes != 6) {
        // For each non-terminal, update its follow set.
        // (Again, we pass the underlying array from the vector.)
        for (int i = NONTERMINALS_START; i < NONTERMINALS_END; i++) {
            FollowSet(i, G, F->followset.data[i].data, F, check);
        }
        passes++;
    }
    return F;
}


// Takes input as G, computes the FIRST and FOLLOW sets
// First_Follow ComputeFirstAndFollowSets(grammar G)
// {
//     First_Follow F = (First_Follow)malloc(sizeof(First_Follow));
//     F->firstset = (int **)malloc(TERMS_SIZE * sizeof(int *));
//     for (int i = 0; i < TERMS_SIZE; i++)
//     {
//         F->firstset[i] = (int *)malloc(20 * sizeof(int));
//         for (int j = 0; j < 20; j++)
//         {
//             F->firstset[i][j] = 0;
//         }
//     }
//     for (int i = 1; i < TERMS_SIZE; i++)
//     {
//         FirstSet(i, G, F->firstset[i], F);
//     }
//     F->followset = (int **)malloc(TERMS_SIZE * sizeof(int *));

//     for (int i = 1; i < NONTERMINALS_START; i++)
//     {
//         F->followset[i] = NULL;
//     }
    

//     for (int i = NONTERMINALS_START; i < NONTERMINALS_END; i++)
//     {
//         F->followset[i] = (int *)malloc(20 * sizeof(int));
//         for (int j = 0; j < 20; j++)
//         {
//             F->followset[i][j] = 0;
//         }
//     }
//     int check[111] = {0};
//     int passes = 1;
//     while (passes != 6)
//     {
//         for (int i = NONTERMINALS_START; i < NONTERMINALS_END; i++)
//         {
//             FollowSet(i, G, F->followset[i], F, check);
//         }
//         passes++;
//     }
//     return F;
// }


void freeFirstandFollow(First_Follow F)
{
    // freeVectorOfVector(&F->firstset);
    // freeVectorOfVector(&F->followset);
    free(F);
}


// Frees the memory allocated to the grammar
void freeGrammar(grammar G)
{
    // for (int i = 0; i < GRAMMAR_SIZE; i++)
    // {
    //     free(accessVector(G->Grammar,i));
    // }
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
        int arr1[]={57,58,59,-1};
        Vector *v1 = array_to_vector(arr1, 4);
        pushBackVector(gr, *v1);
    }
    // Production 2: <mainFunction> ===> TK_MAIN <stmts> TK_END
    {
        int arr2[]={59,1,68,2,-1};
        Vector *v2 = array_to_vector(arr2, 5);
        pushBackVector(gr, *v2);
    }
    // Production 3: <otherFunctions> ===> <function> <otherFunctions>
    {
        int arr3[]={58,60,58,-1};
        Vector *v3 = array_to_vector(arr3, 4);
        pushBackVector(gr, *v3);
    }
    // Production 4: <otherFunctions> ===> <eps>
    {
        int arr4[]={58,110,-1};
        Vector *v4 = array_to_vector(arr4, 3);
        pushBackVector(gr, *v4);
    }
    // Production 5: <function> ===> TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
    {
        int arr5[]={60,3,61,62,4,68,2,-1};
        Vector *v5 = array_to_vector(arr5, 8);
        pushBackVector(gr, *v5);
    }
    // Production 6: <input_par> ===> TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    {
        int arr6[]={61,5,6,7,8,63,9,-1};
        Vector *v6 = array_to_vector(arr6, 8);
        pushBackVector(gr, *v6);
    }
    // Production 7: <output_par> ===> TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    {
        int arr7[]={62,10,6,7,8,63,9,-1};
        Vector *v7 = array_to_vector(arr7, 8);
        pushBackVector(gr, *v7);
    }
    // Production 8: <output_par> ===> <eps>
    {
        int arr8[]={62,110,-1};
        Vector *v8 = array_to_vector(arr8, 3);
        pushBackVector(gr, *v8);
    }
    // Production 9: <parameter_list> ===> <dataType> TK_ID <remaining_list>
    {
        int arr9[]={63,64,56,67,-1};
        Vector *v9 = array_to_vector(arr9, 5);
        pushBackVector(gr, *v9);
    }
    // Production 10: <dataType> ===> <primitiveDataType>
    {
        int arr10[] = {64, 65, -1};
        Vector *v10 = array_to_vector(arr10, 3);
        pushBackVector(gr, *v10);
    }
    // Production 11: <dataType> ===> <constructedDataType>
    {
        int arr11[] = {64, 66, -1};
        Vector *v11 = array_to_vector(arr11, 3);
        pushBackVector(gr, *v11);
    }
    // Production 12: <primitiveDataType> ===> TK_INT
    {
        int arr12[] = {65, 11, -1};
        Vector *v12 = array_to_vector(arr12, 3);
        pushBackVector(gr, *v12);
    }
    // Production 13: <primitiveDataType> ===> TK_REAL
    {
        int arr13[] = {65, 12, -1};
        Vector *v13 = array_to_vector(arr13, 3);
        pushBackVector(gr, *v13);
    }
    // Production 14: <constructedDataType> ===> <A> TK_RUID
    {
        int arr14[] = {66, 109, 13, -1};
        Vector *v14 = array_to_vector(arr14, 4);
        pushBackVector(gr, *v14);
    }
    // Production 15: <constructedDataType> ===> TK_RUID
    {
        int arr15[] = {66, 13, -1};
        Vector *v15 = array_to_vector(arr15, 3);
        pushBackVector(gr, *v15);
    }
    // Production 16: <remaining_list> ===> TK_COMMA <parameter_list>
    {
        int arr16[] = {67, 14, 63, -1};
        Vector *v16 = array_to_vector(arr16, 4);
        pushBackVector(gr, *v16);
    }
    // Production 17: <remaining_list> ===> <eps>
    {
        int arr17[] = {67, 110, -1};
        Vector *v17 = array_to_vector(arr17, 3);
        pushBackVector(gr, *v17);
    }
    // Production 18: <stmts> ===> <typeDefinitions> <declarations> <otherStmts> <returnStmt>
    {
        int arr18[] = {68, 69, 76, 79, 104, -1};
        Vector *v18 = array_to_vector(arr18, 6);
        pushBackVector(gr, *v18);
    }
    // Production 19: <typeDefinitions> ===> <actualOrRedefined> <typeDefinitions>
    {
        int arr19[] = {69, 70, 69, -1};
        Vector *v19 = array_to_vector(arr19, 4);
        pushBackVector(gr, *v19);
    }
    // Production 20: <typeDefinitions> ===> <eps>
    {
        int arr20[] = {69, 110, -1};
        Vector *v20 = array_to_vector(arr20, 3);
        pushBackVector(gr, *v20);
    }
    // Production 21: <actualOrRedefined> ===> <typeDefinition>
    {
        int arr21[] = {70, 71, -1};
        Vector *v21 = array_to_vector(arr21, 3);
        pushBackVector(gr, *v21);
    }
    // Production 22: <actualOrRedefined> ===> <definetypestmt>
    {
        int arr22[] = {70, 108, -1};
        Vector *v22 = array_to_vector(arr22, 3);
        pushBackVector(gr, *v22);
    }
    // Production 23: <typeDefinition> ===> TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD
    {
        int arr23[] = {71, 15, 13, 72, 16, -1};
        Vector *v23 = array_to_vector(arr23, 6);
        pushBackVector(gr, *v23);
    }
    // Production 24: <typeDefinition> ===> TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
    {
        int arr24[] = {71, 17, 13, 72, 18, -1};
        Vector *v24 = array_to_vector(arr24, 6);
        pushBackVector(gr, *v24);
    }
    // Production 25: <fieldDefinitions> ===> <fieldDefinition> <fieldDefinition> <moreFields>
    {
        int arr25[] = {72, 73, 73, 75, -1};
        Vector *v25 = array_to_vector(arr25, 5);
        pushBackVector(gr, *v25);
    }
    // Production 26: <fieldDefinition> ===> TK_TYPE <fieldType> TK_COLON TK_FIELDID TK_SEM
    {
        int arr26[] = {73, 19, 74, 20, 21, 4, -1};
        Vector *v26 = array_to_vector(arr26, 7);
        pushBackVector(gr, *v26);
    }
    // Production 27: <fieldType> ===> <primitiveDataType>
    {
        int arr27[] = {74, 65, -1};
        Vector *v27 = array_to_vector(arr27, 3);
        pushBackVector(gr, *v27);
    }
    // Production 28: <fieldType> ===> <constructedDataType>
    {
        int arr28[] = {74, 66, -1};
        Vector *v28 = array_to_vector(arr28, 3);
        pushBackVector(gr, *v28);
    }
    // Production 29: <moreFields> ===> <fieldDefinition> <moreFields>
    {
        int arr29[] = {75, 73, 75, -1};
        Vector *v29 = array_to_vector(arr29, 4);
        pushBackVector(gr, *v29);
    }
    // Production 30: <moreFields> ===> <eps>
    {
        int arr30[] = {75, 110, -1};
        Vector *v30 = array_to_vector(arr30, 3);
        pushBackVector(gr, *v30);
    }
    // Production 31: <declarations> ===> <declaration> <declarations>
    {
        int arr31[] = {76, 77, 76, -1};
        Vector *v31 = array_to_vector(arr31, 4);
        pushBackVector(gr, *v31);
    }
    // Production 32: <declarations> ===> <eps>
    {
        int arr32[] = {76, 110, -1};
        Vector *v32 = array_to_vector(arr32, 3);
        pushBackVector(gr, *v32);
    }
    // Production 33: <declaration> ===> TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
    {
        int arr33[] = {77, 19, 64, 20, 56, 78, 4, -1};
        Vector *v33 = array_to_vector(arr33, 8);
        pushBackVector(gr, *v33);
    }
    // Production 34: <global_or_not> ===> TK_COLON TK_GLOBAL
    {
        int arr34[] = {78, 20, 22, -1};
        Vector *v34 = array_to_vector(arr34, 4);
        pushBackVector(gr, *v34);
    }
    // Production 35: <global_or_not> ===> <eps>
    {
        int arr35[] = {78, 110, -1};
        Vector *v35 = array_to_vector(arr35, 3);
        pushBackVector(gr, *v35);
    }
    // Production 36: <otherStmts> ===> <stmt> <otherStmts>
    {
        int arr36[] = {79, 80, 79, -1};
        Vector *v36 = array_to_vector(arr36, 4);
        pushBackVector(gr, *v36);
    }
    // Production 37: <otherStmts> ===> <eps>
    {
        int arr37[] = {79, 110, -1};
        Vector *v37 = array_to_vector(arr37, 3);
        pushBackVector(gr, *v37);
    }
    // Production 38: <stmt> ===> <assignmentStmt>
    {
        int arr38[] = {80, 81, -1};
        Vector *v38 = array_to_vector(arr38, 3);
        pushBackVector(gr, *v38);
    }
    // Production 39: <stmt> ===> <iterativeStmt>
    {
        int arr39[] = {80, 89, -1};
        Vector *v39 = array_to_vector(arr39, 3);
        pushBackVector(gr, *v39);
    }
    // Production 40: <stmt> ===> <conditionalStmt>
    {
        int arr40[] = {80, 90, -1};
        Vector *v40 = array_to_vector(arr40, 3);
        pushBackVector(gr, *v40);
    }
    // Production 41: <stmt> ===> <ioStmt>
    {
        int arr41[] = {80, 92, -1};
        Vector *v41 = array_to_vector(arr41, 3);
        pushBackVector(gr, *v41);
    }
    // Production 42: <stmt> ===> <funCallStmt>
    {
        int arr42[] = {80, 86, -1};
        Vector *v42 = array_to_vector(arr42, 3);
        pushBackVector(gr, *v42);
    }
    // Production 43: <assignmentStmt> ===> <SingleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
    {
        int arr43[] = {81, 82, 23, 93, 4, -1};
        Vector *v43 = array_to_vector(arr43, 6);
        pushBackVector(gr, *v43);
    }
    // Production 44: <SingleOrRecId> ===> TK_ID <option_single_constructed>
    {
        int arr44[] = {82, 56, 83, -1};
        Vector *v44 = array_to_vector(arr44, 4);
        pushBackVector(gr, *v44);
    }
    // Production 45: <option_single_constructed> ===> <oneExpansion> <moreExpansions>
    {
        int arr45[] = {83, 84, 85, -1};
        Vector *v45 = array_to_vector(arr45, 4);
        pushBackVector(gr, *v45);
    }
    // Production 46: <option_single_constructed> ===> <eps>
    {
        int arr46[] = {83, 110, -1};
        Vector *v46 = array_to_vector(arr46, 3);
        pushBackVector(gr, *v46);
    }
    // Production 47: <moreExpansions> ===> <oneExpansion> <moreExpansions>
    {
        int arr47[] = {85, 84, 85, -1};
        Vector *v47 = array_to_vector(arr47, 4);
        pushBackVector(gr, *v47);
    }
    // Production 48: <moreExpansions> ===> <eps>
    {
        int arr48[] = {85, 110, -1};
        Vector *v48 = array_to_vector(arr48, 3);
        pushBackVector(gr, *v48);
    }
    // Production 49: <oneExpansion> ===> TK_DOT TK_FIELDID
    {
        int arr49[] = {84, 49, 21, -1};
        Vector *v49 = array_to_vector(arr49, 4);
        pushBackVector(gr, *v49);
    }
    // Production 50: <funCallStmt> ===> <outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
    {
        int arr50[] = {86, 87, 50, 3, 51, 52, 88, 4, -1};
        Vector *v50 = array_to_vector(arr50, 9);
        pushBackVector(gr, *v50);
    }
    // Production 51: <outputParameters> ===> TK_SQL <idList> TK_SQR TK_ASSIGNOP
    {
        int arr51[] = {87, 8, 106, 9, 23, -1};
        Vector *v51 = array_to_vector(arr51, 6);
        pushBackVector(gr, *v51);
    }
    // Production 52: <outputParameters> ===> <eps>
    {
        int arr52[] = {87, 110, -1};
        Vector *v52 = array_to_vector(arr52, 3);
        pushBackVector(gr, *v52);
    }
    // Production 53: <inputParameters> ===> TK_SQL <idList> TK_SQR
    {
        int arr53[] = {88, 8, 106, 9, -1};
        Vector *v53 = array_to_vector(arr53, 5);
        pushBackVector(gr, *v53);
    }
    // Production 54: <iterativeStmt> ===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
    {
        int arr54[] = {89, 24, 25, 100, 26, 80, 79, 55, -1};
        Vector *v54 = array_to_vector(arr54, 9);
        pushBackVector(gr, *v54);
    }
    // Production 55: <conditionalStmt> ===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart>
    {
        int arr55[] = {90, 27, 25, 100, 26, 28, 80, 79, 91, -1};
        Vector *v55 = array_to_vector(arr55, 10);
        pushBackVector(gr, *v55);
    }
    // Production 56: <elsePart> ===> TK_ELSE <stmt> <otherStmts> TK_ENDIF
    {
        int arr56[] = {91, 29, 80, 79, 30, -1};
        Vector *v56 = array_to_vector(arr56, 6);
        pushBackVector(gr, *v56);
    }
    // Production 57: <elsePart> ===> TK_ENDIF
    {
        int arr57[] = {91, 30, -1};
        Vector *v57 = array_to_vector(arr57, 3);
        pushBackVector(gr, *v57);
    }
    // Production 58: <ioStmt> ===> TK_READ TK_OP <var> TK_CL TK_SEM
    {
        int arr58[] = {92, 31, 25, 101, 26, 4, -1};
        Vector *v58 = array_to_vector(arr58, 7);
        pushBackVector(gr, *v58);
    }
    // Production 59: <ioStmt> ===> TK_WRITE TK_OP <var> TK_CL TK_SEM
    {
        int arr59[] = {92, 32, 25, 101, 26, 4, -1};
        Vector *v59 = array_to_vector(arr59, 7);
        pushBackVector(gr, *v59);
    }
    // Production 60: <arithmeticExpression> ===> <term> <expPrime>
    {
        int arr60[] = {93, 95, 94, -1};
        Vector *v60 = array_to_vector(arr60, 4);
        pushBackVector(gr, *v60);
    }
    // Production 61: <expPrime> ===> <lowPrecedenceOp> <term> <expPrime>
    // Production 61: <expPrime> ===> <lowPrecedenceOp> <term> <expPrime>
    {
        int arr61[] = {94, 98, 95, 94, -1};
        Vector *v61 = array_to_vector(arr61, 5);
        pushBackVector(gr, *v61);
    }
    // Production 62: <expPrime> ===> <eps>
    {
        int arr62[] = {94, 110, -1};
        Vector *v62 = array_to_vector(arr62, 3);
        pushBackVector(gr, *v62);
    }
    // Production 63: <term> ===> <factor> <termPrime>
    {
        int arr63[] = {95, 97, 96, -1};
        Vector *v63 = array_to_vector(arr63, 4);
        pushBackVector(gr, *v63);
    }
    // Production 64: <termPrime> ===> <highPrecedenceOp> <factor> <termPrime>
    {
        int arr64[] = {96, 99, 97, 96, -1};
        Vector *v64 = array_to_vector(arr64, 5);
        pushBackVector(gr, *v64);
    }
    // Production 65: <termPrime> ===> <eps>
    {
        int arr65[] = {96, 110, -1};
        Vector *v65 = array_to_vector(arr65, 3);
        pushBackVector(gr, *v65);
    }
    // Production 66: <factor> ===> TK_OP <arithmeticExpression> TK_CL
    {
        int arr66[] = {97, 25, 93, 26, -1};
        Vector *v66 = array_to_vector(arr66, 5);
        pushBackVector(gr, *v66);
    }
    // Production 67: <factor> ===> <var>
    {
        int arr67[] = {97, 101, -1};
        Vector *v67 = array_to_vector(arr67, 3);
        pushBackVector(gr, *v67);
    }
    // Production 68: <lowPrecedenceOp> ===> TK_PLUS
    {
        int arr68[] = {98, 33, -1};
        Vector *v68 = array_to_vector(arr68, 3);
        pushBackVector(gr, *v68);
    }
    // Production 69: <lowPrecedenceOp> ===> TK_MINUS
    {
        int arr69[] = {98, 34, -1};
        Vector *v69 = array_to_vector(arr69, 3);
        pushBackVector(gr, *v69);
    }
    // Production 70: <highPrecedenceOp> ===> TK_MUL
    {
        int arr70[] = {99, 35, -1};
        Vector *v70 = array_to_vector(arr70, 3);
        pushBackVector(gr, *v70);
    }
    // Production 71: <highPrecedenceOp> ===> TK_DIV
    {
        int arr71[] = {99, 36, -1};
        Vector *v71 = array_to_vector(arr71, 3);
        pushBackVector(gr, *v71);
    }
    // Production 72: <booleanExpression> ===> TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL
    {
        int arr72[] = {100, 25, 100, 26, 102, 25, 100, 26, -1};
        Vector *v72 = array_to_vector(arr72, 9);
        pushBackVector(gr, *v72);
    }
    // Production 73: <booleanExpression> ===> <var> <relationalOp> <var>
    {
        int arr73[] = {100, 101, 103, 101, -1};
        Vector *v73 = array_to_vector(arr73, 5);
        pushBackVector(gr, *v73);
    }
    // Production 74: <booleanExpression> ===> TK_NOT TK_OP <booleanExpression> TK_CL
    {
        int arr74[] = {100, 37, 25, 100, 26, -1};
        Vector *v74 = array_to_vector(arr74, 6);
        pushBackVector(gr, *v74);
    }
    // Production 75: <var> ===> <SingleOrRecId>
    {
        int arr75[] = {101, 82, -1};
        Vector *v75 = array_to_vector(arr75, 3);
        pushBackVector(gr, *v75);
    }
    // Production 76: <var> ===> TK_NUM
    {
        int arr76[] = {101, 53, -1};
        Vector *v76 = array_to_vector(arr76, 3);
        pushBackVector(gr, *v76);
    }
    // Production 77: <var> ===> TK_RNUM
    {
        int arr77[] = {101, 54, -1};
        Vector *v77 = array_to_vector(arr77, 3);
        pushBackVector(gr, *v77);
    }
    // Production 78: <logicalOp> ===> TK_AND
    {
        int arr78[] = {102, 38, -1};
        Vector *v78 = array_to_vector(arr78, 3);
        pushBackVector(gr, *v78);
    }
    // Production 79: <logicalOp> ===> TK_OR
    {
        int arr79[] = {102, 39, -1};
        Vector *v79 = array_to_vector(arr79, 3);
        pushBackVector(gr, *v79);
    }
    // Production 80: <relationalOp> ===> TK_LT
    {
        int arr80[] = {103, 40, -1};
        Vector *v80 = array_to_vector(arr80, 3);
        pushBackVector(gr, *v80);
    }
    // Production 81: <relationalOp> ===> TK_LE
    {
        int arr81[] = {103, 41, -1};
        Vector *v81 = array_to_vector(arr81, 3);
        pushBackVector(gr, *v81);
    }
    // Production 82: <relationalOp> ===> TK_EQ
    {
        int arr82[] = {103, 42, -1};
        Vector *v82 = array_to_vector(arr82, 3);
        pushBackVector(gr, *v82);
    }
    // Production 83: <relationalOp> ===> TK_GT
    {
        int arr83[] = {103, 43, -1};
        Vector *v83 = array_to_vector(arr83, 3);
        pushBackVector(gr, *v83);
    }
    // Production 84: <relationalOp> ===> TK_GE
    {
        int arr84[] = {103, 44, -1};
        Vector *v84 = array_to_vector(arr84, 3);
        pushBackVector(gr, *v84);
    }
    // Production 85: <relationalOp> ===> TK_NE
    {
        int arr85[] = {103, 45, -1};
        Vector *v85 = array_to_vector(arr85, 3);
        pushBackVector(gr, *v85);
    }
    // Production 86: <returnStmt> ===> TK_RETURN <optionalReturn> TK_SEM
    {
        int arr86[] = {104, 46, 105, 4, -1};
        Vector *v86 = array_to_vector(arr86, 5);
        pushBackVector(gr, *v86);
    }
    // Production 87: <optionalReturn> ===> TK_SQL <idList> TK_SQR
    {
        int arr87[] = {105, 8, 106, 9, -1};
        Vector *v87 = array_to_vector(arr87, 5);
        pushBackVector(gr, *v87);
    }
    // Production 88: <optionalReturn> ===> <eps>
    {
        int arr88[] = {105, 110, -1};
        Vector *v88 = array_to_vector(arr88, 3);
        pushBackVector(gr, *v88);
    }
    // Production 89: <idList> ===> TK_ID <more_ids>
    {
        int arr89[] = {106, 56, 107, -1};
        Vector *v89 = array_to_vector(arr89, 4);
        pushBackVector(gr, *v89);
    }
    // Production 90: <more_ids> ===> TK_COMMA <idList>
    {
        int arr90[] = {107, 14, 106, -1};
        Vector *v90 = array_to_vector(arr90, 4);
        pushBackVector(gr, *v90);
    }
    // Production 91: <more_ids> ===> <eps>
    {
        int arr91[] = {107, 110, -1};
        Vector *v91 = array_to_vector(arr91, 3);
        pushBackVector(gr, *v91);
    }
    // Production 92: <definetypestmt> ===> TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID
    {
        int arr92[] = {108, 47, 109, 13, 48, 13, -1};
        Vector *v92 = array_to_vector(arr92, 7);
        pushBackVector(gr, *v92);
        // printVector(v92);
    }
    // Production 93: <A> ===> TK_RECORD
    {
        int arr93[] = {109, 15, -1};
        Vector *v93 = array_to_vector(arr93, 3);
        pushBackVector(gr, *v93);
    }
    // Production 94: <A> ===> TK_UNION
    {
        int arr94[] = {109, 17, -1};
        Vector *v94 = array_to_vector(arr94, 3);
        pushBackVector(gr, *v94);
    }
    // printVectorOfVector(gr);

    grammar G = (grammar)malloc(sizeof(struct grammar));
    G->Grammar = (VectorOfVector *)malloc(sizeof(VectorOfVector));
    initVectorOfVector(G->Grammar);
    // G->Grammar=gr;
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


// void parser_main(char *testcaseFile, char *outputFile)
// {
//     // if (parser_initialized == 0)
//     // {
//     //     G = initialize_grammar();
//     //     F = ComputeFirstAndFollowSets(G);
//     //     printf("\nFirst and Follow sets automated \n");
//     //     T = initialize_table(F);
//     //     createParseTable(F, T);
//     //     parser_initialized = 1;
//     // }
//     // char dummy[5] = "----\0";
//     // parseTree root;
//     // root = parseInputSourceCode(testcaseFile, T);
//     // if (syntactically_correct)
//     //     printParseTree(root, outputFile);
//     // else
//     // {
//     //     FILE* fp=fopen(outputFile,"w");
//     //     if(fp==NULL){
//     //         printf("Error opening the file\n");
//     //         exit(1);
//     //     }
//     //     // fprintf(fp," ");
//     //     fclose(fp);
//     //     printf("Code is syntactically incorrect so parse tree could not be constructed\n\n");
//     // }
//         printf("Initializing grammar...\n");
//     grammar G = initialize_grammar();
//     // print_grammar(G);
// }
// TODO: Remove this main function before submitting the assignment.
// this main function is just for testing the grammar initialization.
void parser_main() {
    // char *testFile = "testcase.txt";
    // char *outputFile = "output.txt";
    
    printf("Initializing grammar...\n");
    grammar G = initialize_grammar();
    // print_grammar(G);
    
    printf("Computing FIRST and FOLLOW sets...\n");
    First_Follow F = ComputeFirstAndFollowSets(G);
    printf("hello this is testing of first..\n");
    print_first_set(F, G);
    printf("hello this is testing of follow..\n");
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
    // // freeParseTree(root);
    freeFirstandFollow(F);
    // // freeTable(T);
    freeGrammar(G);
    // freeDataStructures();
    
    printf("Testing complete!\n");
    return 0;
}