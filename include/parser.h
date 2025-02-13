#ifndef PARSER_H
#define PARSER_H

#include "vectorofvector.h"

#define GRAMMAR_SIZE 91      // Total number of productions
#define MAX_PROD_LENGTH 11   // (For documentation only)

// Function prototype for initializing the grammar.
// This creates a VectorOfVector where each production is stored as a Vector.
VectorOfVector *initialize_grammar();
int parser_main();

#endif // PARSER_H
