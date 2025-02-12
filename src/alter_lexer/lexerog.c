#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//-------------------
// Token Types
//-------------------
// order should be strictly same as the order of the tokens in the enum
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

const char *TokenStr[] = {
    "TK_MINUS",
    "TK_IF",
    "TK_DIV",
    "TK_CALL",
    "TK_WITH",
    "TK_MAIN",
    "TK_ENDWHILE",
    "TK_TYPE",
    "TK_ELSE",
    "TK_INT",
    "TK_NOT",
    "TK_GT",
    "TK_PARAMETERS",
    "TK_THEN",
    "TK_SEM",
    "TK_RETURN",
    "TK_ID",
    "TK_DEFINETYPE",
    "TK_OP",
    "TK_WHILE",
    "TK_OR",
    "TK_FUNID",
    "TK_COMMA",
    "TK_INPUT",
    "TK_UNION",
    "TK_RECORD",
    "TK_DOT",
    "TK_RUID",
    "TK_WRITE",
    "TK_ENDUNION",
    "TK_FIELDID",
    "TK_LIST",
    "TK_AND",
    "TK_CL",
    "TK_ASSIGNOP",
    "TK_OUTPUT",
    "TK_EQ",
    "TK_ENDIF",
    "TK_GLOBAL",
    "TK_AS",
    "TK_COLON",
    "TK_NE",
    "TK_GE",
    "TK_LE",
    "TK_SQR",
    "TK_END",
    "TK_PLUS",
    "TK_ENDRECORD",
    "TK_READ",
    "TK_LT",
    "TK_SQL",
    "TK_NUM",
    "TK_RNUM",
    "TK_PARAMETER",
    "TK_MUL",
    "TK_REAL",
    "TK_EPS",
    "TK_EOF",
    "TK_COMMENT",
    "TK_ERR"};

typedef struct
{
    TokenType type;
    char lexeme[100]; // store the actual lexeme
    int lineNo;       // line number

} Token;

#define BUFFER_SIZE 8
#define EOF_SENTINEL '\0'

static FILE *source = NULL;
static char buffer[2][BUFFER_SIZE + 1];
static int currentBuffer = 0;
static int forwardPointer = 0;
static int charsInBuffer[2] = {0, 0};

// We also keep a global line number
static int lineNo = 1;
static int current_s = 0;

int getState(char c, int current_state);
int getStateDetails(int state);
//------------------------------------
// Twin Buffer Helper Functions
//------------------------------------
static int loadBuffer(int whichBuffer)
{
    if (!source)
        return 0;
    size_t bytesRead = fread(buffer[whichBuffer], sizeof(char), BUFFER_SIZE, source);
    buffer[whichBuffer][bytesRead] = EOF_SENTINEL;
    charsInBuffer[whichBuffer] = (int)bytesRead;
    return (int)bytesRead;
}

static void switchBuffer()
{
    currentBuffer = 1 - currentBuffer;
    forwardPointer = 0;
    loadBuffer(currentBuffer);
}

static char getNextCharFromBuffer()
{
    char ch = buffer[currentBuffer][forwardPointer];
    if (ch == EOF_SENTINEL)
    {
        // If we read fewer than BUFFER_SIZE last time, it's true EOF
        if (charsInBuffer[currentBuffer] < BUFFER_SIZE)
            return EOF;
        // Else switch buffer
        switchBuffer();
        ch = buffer[currentBuffer][forwardPointer];
        if (ch == EOF_SENTINEL)
            return EOF;
    }
    forwardPointer++;

    int temp = getState(ch, current_s);
    if (temp != -1)
        current_s = temp;
    else
        current_s = 0;
    // printf("CURRENT STATE : %d for %c \n", current_s, ch);

    if (getStateDetails(current_s) != 0)
    {
        current_s = 0;
    }

    return ch;
}

static void retract(int n)
{
    forwardPointer -= n;
    if (forwardPointer < 0)
    {
        currentBuffer = 1 - currentBuffer;
        forwardPointer = charsInBuffer[currentBuffer] + forwardPointer;
        if (forwardPointer < 0)
        {
            fprintf(stderr, "Error: Retraction too large!\n");
            forwardPointer = 0;
        }
    }
}

//------------------------------------
// 4. Keyword/Token Lookup
//------------------------------------
typedef struct
{
    const char *lexeme;
    TokenType type;
} KeywordEntry;

static KeywordEntry keywordTable[] = {
    {"else", TK_ELSE},
    {"with", TK_WITH},
    {"parameters", TK_PARAMETERS},
    {"output", TK_OUTPUT},
    {"int", TK_INT},
    {"read", TK_READ},
    {"write", TK_WRITE},
    {"return", TK_RETURN},
    {"then", TK_THEN},
    {"real", TK_REAL},
    {"endwhile", TK_ENDWHILE},
    {"if", TK_IF},
    {"type", TK_TYPE},
    {"_main", TK_MAIN},
    {"global", TK_GLOBAL},
    {"endif", TK_ENDIF},
    {"endunion", TK_ENDUNION},
    {"definetype", TK_DEFINETYPE},
    {"as", TK_AS},
    {"call", TK_CALL},
    {"record", TK_RECORD},
    {"endrecord", TK_ENDRECORD},
    {"parameter", TK_PARAMETER},
    {"end", TK_END},
    {"while", TK_WHILE},
    {"union", TK_UNION},
    {"list", TK_LIST},
    {"input", TK_INPUT},
    {NULL, TK_ID} // Sentinel
};

// Look up a lexeme in the keyword table
TokenType lookupKeyword(const char *lex)
{
    for (int i = 0; keywordTable[i].lexeme != NULL; i++)
    {
        if (strcmp(lex, keywordTable[i].lexeme) == 0)
            return keywordTable[i].type;
    }
    return TK_ID; // default
}

//------------------------------------
// 5. Public functions to init/close
//------------------------------------
void initLexer(FILE *fp)
{
    source = fp;
    if (!source)
    {
        fprintf(stderr, "Source file is NULL!\n");
        exit(EXIT_FAILURE);
    }
    currentBuffer = 0;
    forwardPointer = 0;
    lineNo = 1;

    loadBuffer(currentBuffer); // load first buffer
}

void closeLexer()
{
    if (source)
    {
        fclose(source);
        source = NULL;
    }
}

//------------------------------------
// 6. The FSM: getToken()
//------------------------------------

/*
   Pseudocode for numeric token recognition, derived from the regexes:

    1) [0-9]+                  -> TK_NUM
    2) [0-9]+ "." [0-9]+       -> TK_RNUM
    3) [0-9]+ "." [0-9]+ (E|e) [ + | - | ε ] [0-9]+ -> TK_RNUM
*/

// 5000.703?
Token handleNumericLiteral(char firstChar)
{
    Token token;
    int lexPos = 0;
    char c = firstChar;

    // We already know firstChar is a digit
    token.lexeme[lexPos++] = c;

    // ---- STEP 1: Read all remaining digits (the "integer" part) ----
    while (1)
    {
        c = getNextCharFromBuffer();
        if (isdigit((unsigned char)c))
        {
            token.lexeme[lexPos++] = c;
        }
        else
        {
            // we've gone one char too far
            retract(1);
            break;
        }
    }

    // Keep track of whether we have seen a decimal point or exponent
    bool seenDecimal = false;
    bool seenExponent = false;

    // ---- STEP 2: Check if there's a decimal point ----
    c = getNextCharFromBuffer();
    if (c == '.')
    {
        seenDecimal = true;
        token.lexeme[lexPos++] = c; // store '.'

        // We expect at least one digit after the decimal point
        c = getNextCharFromBuffer();
        if (!isdigit((unsigned char)c))
        {
            retract(1);
            token.lexeme[lexPos] = '\0';
            token.type = TK_ERR;
            token.lineNo = lineNo;
            return token;
        }

        // read first digit after the decimal point
        token.lexeme[lexPos++] = c;

        int afterdecimal = 1;
        // read subsequent digits
        while (1)
        {
            c = getNextCharFromBuffer();
            if (isdigit((unsigned char)c))
            {
                token.lexeme[lexPos++] = c;
                afterdecimal++;
            }
            else
            {
                retract(1);
                break;
            }
        }
        if (afterdecimal != 2)
        {
            token.lexeme[lexPos] = '\0';
            token.type = TK_ERR;
            token.lineNo = lineNo;
            return token;
        }
    }
    else
    {
        // no decimal point, so we retrace
        retract(1);
    }

    // ---- STEP 3: If we saw a decimal point, check for exponent part (E/e) ----
    if (seenDecimal)
    {
        c = getNextCharFromBuffer();
        if (c == 'e' || c == 'E')
        {
            seenExponent = true;
            token.lexeme[lexPos++] = c;

            // Next char could be +, -, or digit
            c = getNextCharFromBuffer();
            if (c == '+' || c == '-')
            {
                token.lexeme[lexPos++] = c;
                c = getNextCharFromBuffer();
            }

            if (!isdigit((unsigned char)c))
            {
                // Error: exponent with no digits
                // You might want to handle it or revert
                retract(1);
                // remove E or sign from lexeme if you want
                // but here let's revert to just a decimal token:
                lexPos--; // remove the last getNextChar
                while (lexPos > 0 && (token.lexeme[lexPos - 1] == 'E' ||
                                      token.lexeme[lexPos - 1] == 'e' ||
                                      token.lexeme[lexPos - 1] == '+' ||
                                      token.lexeme[lexPos - 1] == '-'))
                {
                    lexPos--;
                }
                token.lexeme[lexPos] = '\0';
                token.type = TK_RNUM;
                token.lineNo = lineNo;
                return token;
            }

            // we have at least one digit in exponent
            token.lexeme[lexPos++] = c;
            while (1)
            {
                c = getNextCharFromBuffer();
                if (isdigit((unsigned char)c))
                {
                    token.lexeme[lexPos++] = c;
                }
                else
                {
                    retract(1);
                    break;
                }
            }
        }
        else
        {
            // no exponent, retract
            retract(1);
        }
    }

    // ---- STEP 4: Finalize token type and return ----
    token.lexeme[lexPos] = '\0';

    if (seenDecimal)
    {
        // If we had a decimal point, it's TK_RNUM
        token.type = TK_RNUM;
    }
    else
    {
        // Otherwise, it's an integer
        token.type = TK_NUM;
    }

    token.lineNo = lineNo;
    return token;
}

Token handleRecordUnionId(char firstChar)
{
    Token token;
    int lexPos = 0;

    // We already know firstChar == '#'
    token.lexeme[lexPos++] = firstChar;

    // 1) We expect at least ONE lowercase letter
    char c = getNextCharFromBuffer();
    if (c < 'a' || c > 'z')
    {
        // Error: expected at least one letter after '#'
        retract(1);
        token.lexeme[lexPos] = '\0';
        token.type = TK_ERR;
        token.lineNo = lineNo;
        return token;
    }
    token.lexeme[lexPos++] = c;

    // 2) Now read zero or more letters
    while (true)
    {
        c = getNextCharFromBuffer();
        if (c >= 'a' && c <= 'z')
        {
            token.lexeme[lexPos++] = c;
        }
        else
        {
            retract(1);
            break;
        }
    }

    // 3) Finalize token
    token.lexeme[lexPos] = '\0';
    token.type = TK_RUID; // your chosen token type
    token.lineNo = lineNo;

    return token;
}

// Example function to recognize "_[a-zA-Z][a-zA-Z]*[0-9]*" -> TK_FUNID
Token handleFunctionID(char firstChar)
{
    Token token;
    int lexPos = 0;
    char c = firstChar;

    // We already know firstChar == '_' from the caller
    token.lexeme[lexPos++] = c; // store '_'

    // 1) Must see at least one letter
    c = getNextCharFromBuffer();
    if (!isalpha((unsigned char)c))
    {
        // Error: expected [a-zA-Z] after '_'
        retract(1); // retract the non-alpha
        token.lexeme[lexPos] = '\0';
        token.type = TK_ERR;
        token.lineNo = lineNo;
        return token;
    }
    // store that one letter
    token.lexeme[lexPos++] = c;

    // 2) Read zero or more letters
    while (true)
    {
        c = getNextCharFromBuffer();
        if (isalpha((unsigned char)c))
        {
            token.lexeme[lexPos++] = c;
        }
        else
        {
            retract(1);
            break;
        }
    }

    // 3) Read zero or more digits
    while (true)
    {
        c = getNextCharFromBuffer();
        if (isdigit((unsigned char)c))
        {
            token.lexeme[lexPos++] = c;
        }
        else
        {
            retract(1);
            break;
        }
    }

    // Finalize token
    token.lexeme[lexPos] = '\0';
    token.type = TK_FUNID;
    token.lineNo = lineNo;

    // Look up in keyword table
    TokenType ttype = lookupKeyword(token.lexeme);
    if (ttype != TK_ID)
    {
        token.type = ttype;
    }

    return token;
}

Token handleAlpha(char firstChar)
{
    Token token;
    int pos = 0;
    token.lexeme[pos++] = firstChar;

    // 1) Check for [b-d] followed by [2-7]
    //    If yes, parse TK_ID of the form: [b-d][2-7][b-d]*[2-7]*
    if (firstChar >= 'b' && firstChar <= 'd')
    {
        char c = getNextCharFromBuffer();
        if (c >= '2' && c <= '7')
        {
            // We have matched [b-d][2-7]
            token.lexeme[pos++] = c;

            // Read zero or more letters in [b-d]
            while (true)
            {
                c = getNextCharFromBuffer();
                if (c >= 'b' && c <= 'd')
                {
                    token.lexeme[pos++] = c;
                }
                else
                {
                    retract(1);
                    break;
                }
            }

            // Read zero or more digits in [2-7]
            while (true)
            {
                c = getNextCharFromBuffer();
                if (c >= '2' && c <= '7')
                {
                    token.lexeme[pos++] = c;
                }
                else
                {
                    retract(1);
                    break;
                }
            }

            // Done with TK_ID
            token.lexeme[pos] = '\0';
            token.type = TK_ID;
            token.lineNo = lineNo;
            return token;
        }
        else
        {
            // Not [2-7]—so retract and fall back to plain letter token
            retract(1);
        }
    }

    // 2) Otherwise, parse a plain letter sequence [a-z]+
    //    Then lookup in the keyword table; if not found, it's TK_FIELDID
    while (true)
    {
        char c = getNextCharFromBuffer();
        if (c >= 'a' && c <= 'z')
        {
            token.lexeme[pos++] = c;
        }
        else
        {
            // not a letter, so retract it
            retract(1);
            break;
        }
    }
    token.lexeme[pos] = '\0';

    // Look up in keyword table
    TokenType ttype = lookupKeyword(token.lexeme);
    if (ttype != TK_ID) // TK_ID is default lookup value
    {
        // It's a known keyword
        token.type = ttype;
    }
    else
    {
        // Otherwise, treat as TK_FIELDID
        token.type = TK_FIELDID;
    }

    token.lineNo = lineNo;
    return token;
}

int getState(char c, int current_state)
{

    if (current_state == 0)
        switch (c)
        {
        case '%':
            return 1;
        case EOF:
            return 3;
        case '*':
            return 4;
        case '&':
            return 5;
        case '~':
            return 8;
        case '\n':
            return 9;
        case '@':
            return 10;
        case '[':
            return 13;
        case '-':
            return 14;
        case '+':
            return 15;
        case '(':
            return 16;
        case '=':
            return 17;
        case ')':
            return 19;
        case ';':
            return 20;
        case ':':
            return 21;
        case '!':
            return 22;
        case '/':
            return 24;
        case '.':
            return 25;
        case ']':
            return 26;
        case '\t':
        case ' ':
            return 27;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return 29;
        case '#':
            return 40;
        case '_':
            return 43;
        case ',':
            return 64;

        case 'b':
        case 'c':
        case 'd':
            return 47;

        case 'a':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
            return 48;
        case '>':
            return 54;
        case '<':
            return 57;

        default:
            return -1;
        }
    else
    {

        if (current_state == 1){
            if (c == '\n')
                return 2;
            else
                return 1;
        }
        if (current_state == 5 && c == '&')
            return 6;
        if (current_state == 6 && c == '&')
            return 7;
        if (current_state == 10 && c == '@')
            return 11;
        if (current_state == 11 && c == '@')
            return 12;
        if (current_state == 17 && c == '=')
            return 18;
        if (current_state == 22 && c == '=')
            return 23;
        if (current_state == 27)
        {
            if (c == '\t' || c == ' ')
            {
                return 27;
            }
            else
                return 28;
        }
        if (current_state == 29)
        {
            if (c >= '0' && c <= '9')
                return 29;
            else if (c == '.')
                return 30;
            else
                return 39;
        }
        if (current_state == 30)
        {
            if (c >= '0' && c <= '9')
                return 31;
            else
                return 38;
        }
        if (current_state == 31 && c >= '0' && c <= '9')
            return 32;

        if (current_state == 32)
        {
            if (c == 'E' || c == 'e')
                return 33;
            else
                return 37;
        }
        if (current_state == 33)
        {
            if (c == '+' || c == '-')
            {
                return 34;
            }
            else if (c >= '0' && c <= '9')
            {
                return 35;
            }
        }

        if (current_state == 34)
        {
            if (c >= '0' && c <= '9')
            {
                return 35;
            }
        }

        if (current_state == 35)
        {
            if (c >= '0' && c <= '9')
            {
                return 36;
            }
        }

        if (current_state == 40 && c >= 'a' && c <= 'z')
        {
            return 41;
        }
        if (current_state == 41)
        {
            if (c >= 'a' && c <= 'z')
                return 41;
            else
                return 42;
        }

        if (current_state == 43 && ((c >= 'a' && c <= 'z') || c >= 'A' && c <= 'Z'))
        {
            return 44;
        }

        if (current_state == 44)
        {
            if (((c >= 'a' && c <= 'z') || c >= 'A' && c <= 'Z'))
                return 44;
            else if (c >= '0' && c <= '9')
                return 45;
            else
                return 46;
        }

        if (current_state == 45)
        {
            if (c >= '0' && c <= '9')
            {
                return 45;
            }
            else
            {
                return 46;
            }
        }

        if (current_state == 47)
        {
            if (c >= 'a' && c <= 'z')
            {
                return 48;
            }
            else if (c >= '2' && c <= '7')
            {
                return 50;
            }
            else
            {
                return 53;
            }
        }
        if (current_state == 48)
        {
            if (c >= 'a' && c <= 'z')
            {
                return 48;
            }
            else
            {
                return 49;
            }
        }

        if (current_state == 50)
        {
            if (c >= 'b' && c <= 'd')
            {
                return 50;
            }
            else if (c >= '2' && c <= '7')
            {
                return 51;
            }
            else
                return 52;
        }

        if (current_state == 51)
        {
            if (c >= '2' && c <= '7')
            {
                return 51;
            }
            else
                return 52;
        }
        if (current_state == 54)
        {
            if (c == '=')
            {
                return 55;
            }
            else
                return 56;
        }
        if (current_state == 57)
        {
            if (c == '=')
            {
                return 58;
            }
            else if (c == '-')
            {
                return 59;
            }
            else
                return 63;
        }

        if (current_state == 59)
        {
            if (c == '-')
            {
                return 60;
            }
            else
            {
                return 62;
            }
        }
        if (current_state == 60)
        {
            if (c == '-')
            {
                return 61;
            }
            else
            {
                return 69; /// kya karna hai yahan bc
            }
        }
        return -1; // trap state TK_ERR
    }
}

int getStateDetails(int state)
{
    switch (state)
    {
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 18:
    case 19:
    case 20:
    case 21:
    case 23:
    case 24:
    case 25:
    case 26:
    case 64:
    case 36:
    case 55:
    case 58:
    case 61:
        return 1; // non retracting state
    case 28:
    case 37:
    case 39:
    case 42:
    case 46:
    case 49:
    case 52:
    case 53:
    case 56:
    case 63:
        return 2; // once retracting state
    case 38:
    case 62:
        return 3; // twice retracting state
    default:
        return 0; // not final state
    }
}

Token getToken()
{
    Token token;
    token.lexeme[0] = '\0';
    token.type = TK_ERR;
    token.lineNo = lineNo;

    char c;
    int lexPos = 0;

    // Skip whitespace
    while ((c = getNextCharFromBuffer()) != EOF && isspace((unsigned char)c))
    {
        if (c == '\n')
            lineNo++;
    }

    // Check for EOF after skipping whitespace
    if (c == EOF)
    {
        token.type = TK_EOF;
        strcpy(token.lexeme, "EOF");
        token.lineNo = lineNo;
        return token;
    }
    if (c == '#')
    {
        return handleRecordUnionId(c);
    }
    if (c == '_')
    {
        return handleFunctionID(c);
    }
    // Example FSM: handle identifiers/keywords
    if (isalpha((unsigned char)c))
    {
        return handleAlpha(c);
    }

    if (isdigit((unsigned char)c))
    {
        return handleNumericLiteral(c);
    }

    // ------- MULTI-CHARACTER OPERATORS --------
    if (c == '>')
    {
        token.lexeme[0] = c;
        // Peek ahead
        char c2 = getNextCharFromBuffer();
        if (c2 == '=')
        {
            // We have @@@
            token.lexeme[1] = c2;
            token.lexeme[2] = '\0';

            token.type = TK_GE;
            token.lineNo = lineNo;
            return token;
        }
        else
        {
            token.lexeme[1] = '\0';
            token.type = TK_GT;
            token.lineNo = lineNo;
            retract(1);
            return token;
        }
    }
    else if (c == '<')
    {
        token.lexeme[0] = c;
        // Peek ahead
        char c2 = getNextCharFromBuffer();

        if (c2 == '-')
        {
            token.lexeme[1] = c2;

            char c3 = getNextCharFromBuffer();
            if (c3 == '-')
            {
                token.lexeme[2] = c3;

                char c4 = getNextCharFromBuffer();
                if (c4 == '-')
                {
                    token.lexeme[3] = c4;
                    token.lexeme[4] = '\0';

                    token.type = TK_ASSIGNOP;
                    token.lineNo = lineNo;
                    return token;
                }
                else
                {
                    // token.lexeme[3] = '\0';
                    // retract(1);
                    // return token;
                    //????????
                }
            }
            else
            {
                token.lexeme[2] = '\0';
                retract(2);
                return token;
            }
        }

        else if (c2 == '=')
        {
            // We have @@@
            token.lexeme[1] = c2;
            token.lexeme[2] = '\0';

            token.type = TK_LE;
            token.lineNo = lineNo;
            return token;
        }
        else
        {
            token.lexeme[1] = '\0';
            token.type = TK_LT;
            token.lineNo = lineNo;
            retract(1);
            return token;
        }
    }

    else if (c == '&')
    {
        token.lexeme[0] = c;

        char c2 = getNextCharFromBuffer();
        if (c2 == '&')
        {
            token.lexeme[1] = c2;
            char c3 = getNextCharFromBuffer();
            if (c3 == '&')
            {
                token.lexeme[2] = c3;
                token.lexeme[3] = '\0';

                token.type = TK_AND;
                token.lineNo = lineNo;
                return token;
            }
            else
            {
                // We only saw "&&" not "&&&"
                token.lexeme[2] = '\0';
                retract(1);
                return token;
            }
        }
        else
        {
            // Single '&' or something else like '&X'
            // Retract the second char so we can handle '&' in single-char switch
            retract(1);
            return token;
        }
    }
    else if (c == '@')
    {
        token.lexeme[0] = '@';
        // Peek ahead
        char c2 = getNextCharFromBuffer();
        if (c2 == '@')
        {
            token.lexeme[1] = '@';
            char c3 = getNextCharFromBuffer();
            if (c3 == '@')
            {
                // We have @@@
                token.lexeme[2] = '@';
                token.lexeme[3] = '\0';

                token.type = TK_OR;
                token.lineNo = lineNo;
                return token;
            }
            else
            {
                // We only saw "@@" not "@@@" so retract 1 chars
                retract(1);
                token.lexeme[2] = '\0';
                return token;
            }
        }
        else
        {
            // Single '@' or something else
            token.lexeme[1] = '\0';
            retract(1);
            return token;
        }
    }
    else if (c == '=')
    {
        token.lexeme[0] = '=';
        // Peek ahead
        char c2 = getNextCharFromBuffer();
        if (c2 == '=')
        {
            // We have @@@
            token.lexeme[1] = '=';
            token.lexeme[2] = '\0';

            token.type = TK_EQ;
            token.lineNo = lineNo;
            return token;
        }
        else
        {

            retract(1);
            token.lexeme[1] = '\0';
            return token;
        }
    }
    else if (c == '!')
    {
        token.lexeme[0] = '!';
        // Peek ahead
        char c2 = getNextCharFromBuffer();
        if (c2 == '=')
        {
            // We have @@@
            token.lexeme[1] = '=';
            token.lexeme[2] = '\0';

            token.type = TK_NE;
            token.lineNo = lineNo;
            return token;
        }
        else
        {
            // We only saw "@@" not "@@@" so retract 1 chars
            token.lexeme[1] = '\0';
            retract(1);
            return token;
        }
    }

    // ------- SINGLE-CHARACTER OPERATORS --------
    token.lexeme[0] = c;
    token.lexeme[1] = '\0';
    token.lineNo = lineNo;

    switch (c)
    {
    case '%':

        while (1)
        {
            c = getNextCharFromBuffer();
            if (c == EOF)
                break; // reached end of file
            if (c == '\n')
            {
                lineNo++; // new line, so update lineNo
                break;    // done skipping the comment line
            }
        }
        token.type = TK_COMMENT;
        break;

    case '*':
        token.type = TK_MUL;
        break;
    case '~':
        token.type = TK_NOT;
        break;
    case '[':
        token.type = TK_SQL;
        break;
    case '-':
        token.type = TK_MINUS;
        break;
    case '+':
        token.type = TK_PLUS;
        break;
    case '(':
        token.type = TK_OP;
        break;
    case ')':
        token.type = TK_CL;
        break;
    case ';':
        token.type = TK_SEM;
        break;
    case ':':
        token.type = TK_COLON;
        break;
    case '/':
        token.type = TK_DIV;
        break;
    case '.':
        token.type = TK_DOT;
        break;
    case ']':
        token.type = TK_SQR;
        break;
    case ',':
        token.type = TK_COMMA;
        break;
    default:
        token.type = TK_ERR;
        break;
    }
    return token;
}

Token newGetToken()
{

    Token token;
    token.lexeme[0] = '\0';
    token.type = TK_ERR;
    token.lineNo = lineNo;

    char ch = getNextCharFromBuffer();
    int temp = getState(ch, current_s);
    int st = getStateDetails(temp);

    if (st == 1)
    {
        current_s = 0;
    }
    else if (st == 2)
    {
        retract(1);
        current_s = 0;
    }
    else if (st == 3)
    {
        retract(2);
        current_s = 0;
    }
    else
    {
        newGetToken();
    }
}
// Function to convert enum to string
const char *getTokenStr(TokenType t)
{
    if (t >= TK_MINUS && t <= TK_ERR)
    {
        return TokenStr[t];
    }
    return "UNKNOWN_TOKEN";
}

int printToken(Token t)
{

    if (t.type == TK_EOF)
    {
        return -1;
    }
    if (t.type == TK_ERR)
    {
        printf("Line no. %d\t Error: Unknown pattern <%s> \n", t.lineNo, t.lexeme);
    }
    else
    {
        const char *tokenName = NULL;
        tokenName = getTokenStr(t.type);
        printf("Line no. %d\t Lexeme %-10s\t Token %s\n", t.lineNo, t.lexeme, tokenName);
    }
    return 1;
}

int main(int argc, char **argv)
{
    FILE *fp = NULL;
    if (argc > 1)
    {
        fp = fopen(argv[1], "r");
        if (!fp)
        {
            perror("Failed to open file");
            return 1;
        }
    }
    else
    {
        // no file given, read from stdin
        fp = stdin;
    }

    initLexer(fp);

    while (1)
    {
        Token t = getToken();
        if (printToken(t) == -1)
        {
            break;
        }
    }

    closeLexer();
    return 0;
}
