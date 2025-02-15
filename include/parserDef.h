#ifndef PARSERDEF_H
#define PARSERDEF_H

#define NONTERMINALS 56    
#define TERMINALS 53      // Changed from 57
#define TERMS_SIZE 110     // Changed from 111
#define EPLS 109          // Changed from 110
#define Grammer 94
// Terminals
"TK_MAIN",                  // 0
"TK_END",                   // 1
"TK_FUNID",                 // 2
"TK_SEM",                   // 3
"TK_INPUT",                 // 4
"TK_PARAMETER",             // 5
"TK_LIST",                  // 6
"TK_SQL",                   // 7
"TK_SQR",                   // 8
"TK_OUTPUT",                // 9
"TK_INT",                   // 10
"TK_REAL",                  // 11
"TK_RUID",                  // 12
"TK_COMMA",                 // 13
"TK_RECORD",                // 14
"TK_ENDRECORD",             // 15
"TK_UNION",                 // 16
"TK_ENDUNION",              // 17
"TK_TYPE",                  // 18
"TK_COLON",                 // 19
"TK_FIELDID",               // 20
"TK_GLOBAL",                // 21
"TK_ASSIGNOP",              // 22
"TK_WHILE",                 // 23
"TK_OP",                    // 24
"TK_CL",                    // 25
"TK_IF",                    // 26
"TK_THEN",                  // 27
"TK_ELSE",                  // 28
"TK_ENDIF",                 // 29
"TK_READ",                  // 30
"TK_WRITE",                 // 31
"TK_PLUS",                  // 32
"TK_MINUS",                 // 33
"TK_MUL",                   // 34
"TK_DIV",                   // 35
"TK_NOT",                   // 36
"TK_AND",                   // 37
"TK_OR",                    // 38
"TK_LT",                    // 39
"TK_LE",                    // 40
"TK_EQ",                    // 41
"TK_GT",                    // 42
"TK_GE",                    // 43
"TK_NE",                    // 44
"TK_RETURN",                // 45
"TK_DEFINETYPE",            // 46
"TK_AS",                    // 47
"TK_DOT",                   // 48
"TK_CALL",                  // 49
"TK_WITH",                  // 50
"TK_PARAMETERS",            // 51
"TK_NUM",                   // 52
"TK_RNUM",                  // 53
"TK_ENDWHILE",              // 54
"TK_ID",                    // 55
// Non-Terminals
"program",                  // 56
"otherFunctions",           // 57
"mainFunction",             // 58
"function",                 // 59
"input_par",                // 60
"output_par",               // 61
"parameter_list",           // 62
"dataType",                 // 63
"primitiveDataType",        // 64
"constructedDataType",      // 65
"remaining_list",           // 66
"stmts",                    // 67
"typeDefinitions",          // 68
"actualOrRedefined",        // 69
"typeDefinition",           // 70
"fieldDefinitions",         // 71
"fieldDefinition",          // 72
"fieldType",                // 73
"moreFields",               // 74
"declarations",             // 75
"declaration",              // 76
"global_or_not",            // 77
"otherStmts",               // 78
"stmt",                     // 79
"assignmentStmt",           // 80
"SingleOrRecId",            // 81
"option_single_constructed",// 82
"oneExpansion",             // 83
"moreExpansions",           // 84
"funCallStmt",              // 85
"outputParameters",         // 86
"inputParameters",          // 87
"iterativeStmt",            // 88
"conditionalStmt",          // 89
"elsePart",                 // 90
"ioStmt",                   // 91
"arithmeticExpression",     // 92
"expPrime",                 // 93
"term",                     // 94
"termPrime",                // 95
"factor",                   // 96
"lowPrecedenceOp",          // 97
"highPrecedenceOp",         // 98
"booleanExpression",        // 99
"var",                      // 100
"logicalOp",                // 101
"relationalOp",             // 102
"returnStmt",               // 103
"optionalReturn",           // 104
"idList",                   // 105
"more_ids",                 // 106
"definetypestmt",           // 107
"A",                        // 108
"eps",                      // 109 (for epsilon productions)
#endif

