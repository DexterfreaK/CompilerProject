/*******************************************************************************
   File: lexer.c
   Description: Code for a twin-buffer based lexical analyzer.

   NOTES:
   1) The code below gives a basic structure for your lexer.
   2) The actual DFA transition logic is left as stubs – fill it in yourself.
   3) The twin-buffer mechanism is shown in simplified form.
   4) The example includes minimal states to demonstrate the approach.
   5) Feel free to rearrange or rename functions, but keep the general idea.
********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFF_SIZE 64 // Size of each half of the twin buffer
#define MAX_LEXEME_SIZE 100

/* --------------------------------------------------------------------------
   ENUM FOR TOKEN TYPES
   In an actual compiler project, you'll have many more token types.
   Fill these in according to your language specification.
-------------------------------------------------------------------------- */
typedef enum
{
    TK_COMMENT,
    TK_MAIN,
    TK_FUNID,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_INT,
    TK_REAL,
    TK_ID,
    TK_COMMA,
    TK_OUTPUT,
    TK_INPUT,
    TK_ASSIGNOP,
    TK_ERROR,
    // ... add the rest as needed ...
    TK_EOF // End Of File
} TokenType;

/* --------------------------------------------------------------------------
   STRUCT FOR A TOKEN
   Holds:
   1) tokenType: enumerated type
   2) lexeme   : the actual string recognized
   3) lineNumber: line on which the token was found
-------------------------------------------------------------------------- */
typedef struct
{
    TokenType tokenType;
    char lexeme[MAX_LEXEME_SIZE];
    int lineNumber;
} Token;

/* --------------------------------------------------------------------------
   GLOBALS for the Twin Buffer Mechanism
-------------------------------------------------------------------------- */
// We maintain two buffers (each of size BUFF_SIZE), plus a file pointer
static char buffer1[BUFF_SIZE];
static char buffer2[BUFF_SIZE];

// This pointer always points to the "current" character in whichever buffer is active
static int forward = 0;

// This tells us whether we are in the first buffer or second buffer
// 0 -> buffer1 is active, 1 -> buffer2 is active
static int currentBuffer = 0;

// We store the file pointer globally for convenience
static FILE *sourceFile = NULL;

// Keep track of the current line number in the source code
static int currentLine = 1;

// A sentinel to mark end of file when reading
static int fileEnded = 0;

/* --------------------------------------------------------------------------
   FUNCTION PROTOTYPES
-------------------------------------------------------------------------- */
void initializeLexer(const char *filename);
void closeLexer();
static void loadBuffer(int whichBuffer);
static char getChar();
static void ungetChar();
Token getNextToken();

/* --------------------------------------------------------------------------
   INITIALIZE THE LEXER
   1) Opens the source file.
   2) Loads the first buffer.
   3) Sets initial states/line numbers/etc.
-------------------------------------------------------------------------- */
void initializeLexer(const char *filename)
{
    sourceFile = fopen(filename, "r");
    if (sourceFile == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    // Initially set line number and no end of file
    currentLine = 1;
    fileEnded = 0;
    forward = 0;
    currentBuffer = 0;

    // Load the first buffer
    loadBuffer(0);
    // We can preemptively load buffer2 as well, or lazily load it only on switch
    // For now, let's do it lazily
}

/* --------------------------------------------------------------------------
   CLOSE THE LEXER
   1) Closes the file.
-------------------------------------------------------------------------- */
void closeLexer()
{
    if (sourceFile)
    {
        fclose(sourceFile);
        sourceFile = NULL;
    }
}

/* --------------------------------------------------------------------------
   loadBuffer(whichBuffer)
   Reads up to BUFF_SIZE characters from the file into the specified buffer.
   If EOF is reached, we mark fileEnded so that getChar can handle it.
-------------------------------------------------------------------------- */
static void loadBuffer(int whichBuffer)
{
    if (fileEnded)
    {
        // Once the file has ended, there's no more to read
        if (whichBuffer == 0)
            buffer1[0] = '\0';
        else
            buffer2[0] = '\0';
        return;
    }

    int bytesRead;
    if (whichBuffer == 0)
    {
        bytesRead = fread(buffer1, sizeof(char), BUFF_SIZE - 1, sourceFile);
        // Null-terminate
        buffer1[bytesRead] = '\0';
    }
    else
    {
        bytesRead = fread(buffer2, sizeof(char), BUFF_SIZE - 1, sourceFile);
        buffer2[bytesRead] = '\0';
    }

    if (bytesRead < BUFF_SIZE - 1)
    {
        // We reached EOF
        fileEnded = 1;
    }
}

/* --------------------------------------------------------------------------
   getChar()
   Returns the next character from the source (from whichever buffer is active).
   If we are at the end of one buffer, we switch to the other, possibly re-filling.
-------------------------------------------------------------------------- */
static char getChar()
{
    char c;

    // Decide which buffer we are reading from
    if (currentBuffer == 0)
    {
        c = buffer1[forward];
        if (c == '\0')
        {
            // Buffer1 ended. Time to switch to buffer2
            currentBuffer = 1;
            loadBuffer(1);
            forward = 0;
            c = buffer2[forward];
        }
    }
    else
    {
        // currentBuffer == 1
        c = buffer2[forward];
        if (c == '\0')
        {
            // Buffer2 ended. Switch to buffer1
            currentBuffer = 0;
            loadBuffer(0);
            forward = 0;
            c = buffer1[forward];
        }
    }

    // If still \0, then we definitely reached EOF
    if (c == '\0')
    {
        return '\0'; // We'll interpret '\0' as end-of-file
    }

    // Move forward
    forward++;

    // Update line number if needed
    if (c == '\n')
    {
        currentLine++;
    }

    return c;
}

/* --------------------------------------------------------------------------
   ungetChar()
   Moves the 'forward' pointer back by one.
   Should only be called if we haven't crossed a buffer boundary.
   Typically used if the DFA overshoots by one character and needs to retract.
-------------------------------------------------------------------------- */
static void ungetChar()
{
    // If we just read a newline, revert the line count
    // (In some implementations you do this only if you recognized the newline as a separate token)
    // For simplicity, let's do it unconditionally if the previous char was '\n'.
    // But we do not know the char we 'unget'. So be mindful.

    // Actually, a robust version might store the last char read in a separate variable
    // to decide if we revert line count. Here we keep it minimal.

    if (forward > 0)
    {
        forward--;
    }
    // If forward == 0, we are at the boundary, which complicates matters:
    //   you'd have to switch to the other buffer.
    // We'll skip that detail in this skeleton for simplicity, but be sure
    // to handle it if your actual logic can retract across a boundary.
}

/* --------------------------------------------------------------------------
   getNextToken()
   This function implements the core of the scanning logic.

   For demonstration, only a tiny portion of the DFA is shown:
   - if we see '%', treat it as a comment token (TK_COMMENT).
   - if we see "_main", produce TK_MAIN.
   - if we see "_input", produce TK_FUNID
   - etc.

   In reality, you will create multiple states in a state machine,
   and use 'getChar' / 'ungetChar' calls as you do so.
-------------------------------------------------------------------------- */
Token getNextToken()
{
    Token newToken;
    newToken.tokenType = TK_ERROR; // default
    newToken.lexeme[0] = '\0';
    newToken.lineNumber = currentLine;

    // This is an extremely simplified example. In practice, you’d implement
    // a full DFA with multiple states, transitions, and “retracts”.

    char c = getChar();
    if (c == '\0')
    {
        // End Of File
        newToken.tokenType = TK_EOF;
        strcpy(newToken.lexeme, "EOF");
        return newToken;
    }

    // Skip whitespace
    while (c == ' ' || c == '\t' || c == '\r' || c == '\n')
    {
        c = getChar();
        if (c == '\0')
        {
            newToken.tokenType = TK_EOF;
            strcpy(newToken.lexeme, "EOF");
            return newToken;
        }
    }

    // Start building a lexeme
    char lexBuf[MAX_LEXEME_SIZE];
    int lexIndex = 0;

    // EXAMPLE 1: Check for comment if we see '%'
    if (c == '%')
    {
        // Suppose we define a rule that a single '%' alone is TK_COMMENT.
        // Or you might skip until newline, etc. For now, let's just do a trivial:
        lexBuf[lexIndex++] = c;
        lexBuf[lexIndex] = '\0';

        newToken.tokenType = TK_COMMENT;
        strcpy(newToken.lexeme, lexBuf);
        newToken.lineNumber = currentLine;
        return newToken;
    }

    // EXAMPLE 2: Check if it’s an underscore '_' starting an identifier like _main, _input, etc.
    if (c == '_')
    {
        // We read the underscore.
        lexBuf[lexIndex++] = c;

        // Let's read ahead to see if we get "main" or "input" or something else
        // This is purely for demonstration. Real logic will be more complex.
        char c2;
        while ((c2 = getChar()) != '\0' && isalpha(c2) && lexIndex < MAX_LEXEME_SIZE - 1)
        {
            lexBuf[lexIndex++] = c2;
        }
        // We might have consumed one character too far
        if (c2 != '\0' && !isspace(c2))
        {
            // If we read a non-space, we might need to ungetChar if it's not part of this lexeme
            ungetChar();
        }
        lexBuf[lexIndex] = '\0';

        if (strcmp(lexBuf, "_main") == 0)
        {
            newToken.tokenType = TK_MAIN;
        }
        else if (strcmp(lexBuf, "_input") == 0)
        {
            newToken.tokenType = TK_FUNID;
        }
        else
        {
            // We can treat all other underscore+alphabetic as TK_FUNID or TK_ID
            // depending on your language rules. For now, let's mark it as TK_FUNID.
            newToken.tokenType = TK_FUNID;
        }
        strcpy(newToken.lexeme, lexBuf);
        newToken.lineNumber = currentLine;
        return newToken;
    }

    // EXAMPLE 3: Check for certain keywords, e.g. “input”
    //   You will typically do a more thorough approach: read all letters, then
    //   compare with known keywords. For demo, let's say if the first letter is 'i',
    //   we keep reading letters to see if we match "input".
    if (isalpha(c))
    {
        // Start building an identifier or keyword
        lexBuf[lexIndex++] = c;
        char c3;
        while ((c3 = getChar()) != '\0' && isalnum(c3) && lexIndex < MAX_LEXEME_SIZE - 1)
        {
            lexBuf[lexIndex++] = c3;
        }
        if (c3 != '\0' && !isspace(c3))
        {
            ungetChar();
        }
        lexBuf[lexIndex] = '\0';

        // Compare with known keywords or treat as ID
        if (strcmp(lexBuf, "input") == 0)
        {
            newToken.tokenType = TK_INPUT;
        }
        else if (strcmp(lexBuf, "output") == 0)
        {
            newToken.tokenType = TK_OUTPUT;
        }
        else if (strcmp(lexBuf, "parameter") == 0)
        {
            newToken.tokenType = TK_PARAMETER;
        }
        else if (strcmp(lexBuf, "list") == 0)
        {
            newToken.tokenType = TK_LIST;
        }
        else if (strcmp(lexBuf, "int") == 0)
        {
            newToken.tokenType = TK_INT;
        }
        else if (strcmp(lexBuf, "real") == 0)
        {
            newToken.tokenType = TK_REAL;
        }
        else
        {
            // Not a recognized keyword => treat it as an identifier
            newToken.tokenType = TK_ID;
        }

        strcpy(newToken.lexeme, lexBuf);
        newToken.lineNumber = currentLine;
        return newToken;
    }

    // EXAMPLE 4: If it’s a single character token like ',' or '[' or ']'
    if (c == ',')
    {
        newToken.tokenType = TK_COMMA;
        newToken.lexeme[0] = c;
        newToken.lexeme[1] = '\0';
        newToken.lineNumber = currentLine;
        return newToken;
    }
    if (c == '[')
    {
        newToken.tokenType = TK_SQL;
        newToken.lexeme[0] = c;
        newToken.lexeme[1] = '\0';
        newToken.lineNumber = currentLine;
        return newToken;
    }
    if (c == ']')
    {
        newToken.tokenType = TK_SQR;
        newToken.lexeme[0] = c;
        newToken.lexeme[1] = '\0';
        newToken.lineNumber = currentLine;
        return newToken;
    }

    // ... and so on for other single/double character tokens, operators, etc. ...

    // If none of the above matched, treat it as an error for demonstration:
    newToken.tokenType = TK_ERROR;
    newToken.lexeme[0] = c;
    newToken.lexeme[1] = '\0';
    newToken.lineNumber = currentLine;

    return newToken;
}

/* --------------------------------------------------------------------------
   SAMPLE MAIN (DRIVER) to demonstrate usage
   In the final compiler, you might have a more elaborate driver that:
     1. Accepts command line args
     2. Offers a menu (0 for exit, 1 for removing comments, 2 for printing tokens, etc.)
     3. Invokes the lexer in different ways
     4. Possibly calls the parser, etc.

   The below is just a minimal demonstration that:
     - Initializes the lexer
     - Reads tokens in a loop
     - Prints them
     - Closes the lexer
-------------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source_code_file>\n", argv[0]);
        return 0;
    }

    initializeLexer(argv[1]);

    Token t;
    while (1) {
        t = getNextToken();
        if (t.tokenType == TK_EOF) {
            printf("Line %d\tLexeme %-15s Token TK_EOF\n", t.lineNumber, t.lexeme);
            break;
        } else if (t.tokenType == TK_ERROR) {
            printf("Line %d\tLexeme %-15s Token TK_ERROR\n", t.lineNumber, t.lexeme);
        } else {
            // Print token type in some readable form:
            // (You might create a function tokenTypeToString(...) for printing.)
            printf("Line %d\tLexeme %-15s Token %d\n", t.lineNumber, t.lexeme, t.tokenType);
        }
    }

    closeLexer();
    return 0;
}

