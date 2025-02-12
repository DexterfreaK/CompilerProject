#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* Macros */
#define TRAP_STATE -1
#define LENGTHLEXEME 22
#define EOF_SENTINEL '\0'

/*-------------------
   Token Types
  -------------------*/
/* Order must match the token order used in lexer.c */
typedef enum
{
    TK_MINUS,
    TK_IF,
    TK_DIV,
    TK_CALL,
    TK_WITH,
    TK_MAIN,
    TK_ENDWHILE,
    TK_TYPE,
    TK_ELSE,
    TK_INT,
    TK_NOT,
    TK_GT,
    TK_PARAMETERS,
    TK_THEN,
    TK_SEM,
    TK_RETURN,
    TK_ID,
    TK_DEFINETYPE,
    TK_OP,
    TK_WHILE,
    TK_OR,
    TK_FUNID,
    TK_COMMA,
    TK_INPUT,
    TK_UNION,
    TK_RECORD,
    TK_DOT,
    TK_RUID,
    TK_WRITE,
    TK_ENDUNION,
    TK_FIELDID,
    TK_LIST,
    TK_AND,
    TK_CL,
    TK_ASSIGNOP,
    TK_OUTPUT,
    TK_EQ,
    TK_ENDIF,
    TK_GLOBAL,
    TK_AS,
    TK_COLON,
    TK_NE,
    TK_GE,
    TK_LE,
    TK_SQR,
    TK_END,
    TK_PLUS,
    TK_ENDRECORD,
    TK_READ,
    TK_LT,
    TK_SQL,
    TK_NUM,
    TK_RNUM,
    TK_PARAMETER,
    TK_MUL,
    TK_REAL,
    TK_EPS,
    TK_EOF,
    TK_COMMENT,
    TK_ERR
} TokenType;

/* For converting tokens to strings if needed */
extern const char *TokenStr[];

/*-------------------
   State Details
  -------------------*/
typedef enum
{
    NON_FINAL = 0,
    FINAL_NO_RETRACT = 1,
    FINAL_RETRACTONCE = 2,
    FINAL_RETRACTTWICE = 3,
} StateDetail;

/*-------------------
   Invalid Token Enum
  -------------------*/
typedef enum
{
  EXIT,
  CONTINUE,
  LENGTHEXCEEDED,
  NORMAL,
  ERROR,

} TokenCategory;

/*-------------------
   Token Structure
  -------------------*/
typedef struct
{
    TokenType type;
    TokenCategory cat;
    char lexeme[100]; // Actual lexeme
    int lineNo;       // Line number in source file
} Token;

/*-------------------
   Twin Buffer Structure
  -------------------*/
typedef struct
{
    FILE *source;       // Input file pointer
    char **buffers;     // Array of two character buffers
    int *charsInBuffer; // Number of valid characters in each buffer
    int currentBuffer;  // Index of the current buffer (0 or 1)
    int forwardPointer; // Current read index within the active buffer
    int bufferSize;     // Size of each buffer
    int lineNo;         // Current line number (if maintained here)
    int current_s;      // Current state (if maintained here)
} twinBuffer;


/*-------------------
   Function Prototypes
  -------------------*/

/* Twin buffer functions */
twinBuffer *createTwinBuffer(FILE *source, int bufSize);
void destroyTwinBuffer(twinBuffer *B);
int getNextCharFromBuffer(twinBuffer *B);
void retract(twinBuffer *B, int n);

/* FSM and lexing functions */
int getState(char c, int current_state);
int getStateDetails(twinBuffer *B, int state);
TokenType lookupKeyword(const char *lex);
void token_fun(Token *token);
void id_fun(Token *token);
void doStateActions(Token *token, int state);
const char *getTokenStr(TokenType t);
void newGetToken(twinBuffer *B, Token *token, int pos);
int printToken(Token *t);

/* Driver function */
void driverToken(char *fn);

/* Utility functions */
void removeComments(const char *fn);

#endif /* LEXER_H */
