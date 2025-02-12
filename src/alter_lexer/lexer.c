#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define TRAP_STATE -1
#define LENGTHLEXEME 50
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

typedef enum
{

    NON_FINAL = 0,
    FINAL_NO_RETRACT = 1,
    FINAL_RETRACTONCE = 2,
    FINAL_RETRACTTWICE = 3,

} StateDetail;

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

#define BUFFER_SIZE 50
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

    // int temp = getState(ch, current_s);
    // if (temp != -1)
    //     current_s = temp;
    // else
    //     current_s = 0;
    // printf("CURRENT STATE : %d for %c \n", current_s, ch);

    // if (getStateDetails(current_s) != 0)
    // {
    //     current_s = 0;
    // }

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


// 5000.703?
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
            return TRAP_STATE;
        }
    else
    {

        if (current_state == 1)
        {
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
        return TRAP_STATE; // trap state TK_ERR
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
        return FINAL_NO_RETRACT; // non retracting state
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
        retract (1);
        return FINAL_RETRACTONCE; // once retracting state
    case 38:
    case 62:
        retract(2);  
        return FINAL_RETRACTTWICE; // twice retracting state


    default:
        return NON_FINAL; // not final state
    }
}
typedef enum
{
    EXIT,
    CONTINUE,
    LENGTHEXCEEDED,
    NOTINVALID,

} InvalidToken;


void token_fun(Token *token)
{
    token->type = lookupKeyword(token->lexeme);
    if(token->type == TK_ID){
        token->type = TK_FIELDID;
    }
}
void id_fun(Token *token)
{
    token->type = lookupKeyword(token->lexeme); // checking for _main else 
    if(token->type == TK_ID){
        token->type = TK_FUNID;
    }
}
int doStateActions(Token *token, int state)
{
    switch (state)
    {
    /* Non‐retracting final states */
    case 2:
        lineNo++;
        token->type = TK_COMMENT;
        break;

    case 4:
        token->type = TK_MUL;
        break;
    case 7:
        token->type = TK_AND;
        break;
    case 8:
        token->type = TK_NOT;
        break;
    case 9:
        lineNo++;
        return CONTINUE;
    case 12:
        token->type = TK_OR;
        break;
    case 13:
        token->type = TK_SQL;
        break;
    case 14:
        token->type = TK_MINUS;
        break;
    case 15:
        token->type = TK_PLUS;
        break;
    case 16:
        token->type = TK_OP;
        break;
    case 18:
        token->type = TK_EQ;
        break;
    case 19:
        token->type = TK_CL;
        break;
    case 20:
        token->type = TK_SEM;
        break;
    case 21:
        token->type = TK_COLON;
        break;
    case 23:
        token->type = TK_NE;
        break;
    case 24:
        token->type = TK_DIV;
        break;
    case 25:
        token->type = TK_DOT;
        break;
    case 26:
        token->type = TK_SQR;
        break;
    
    case 64:
        token->type = TK_COMMA;
        break;
    case 36:
        token->type = TK_RNUM;
        break;
    case 55:
        token->type = TK_GE;
        break;
    case 56:
        token->type = TK_GT;
        break;
    case 58:
        token->type = TK_LE;
        break;
    case 61:
        token->type = TK_ASSIGNOP;
        break;
  
    case 37:
        token->type = TK_NUM;
        break;
    case 38:
        token->type = TK_NUM;
        break;
    case 39:
        token->type = TK_NUM;
        break;

    case 42:
        token->type = TK_RUID;
        break;

    case 52:
        token->type = TK_ID;
        break;
    case 53:
        token->type = TK_FIELDID;
        break;

    case 62:
        token->type = TK_LT;
        break;
    case 63:
        token->type = TK_LT;
        break;

    case 46:
        id_fun(token);
        break;
    case 49:
        token_fun(token);
        break;

    case 3:
        // EOF token
        return EXIT;
    case 28:
        // Whitespace token – do not return a token.
        return CONTINUE;

    default:
        token->type = TK_ERR;
        break;
    }
    return NOTINVALID;
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

int newGetToken(Token* token, int pos)
{
    char ch = getNextCharFromBuffer();
    int nextState = getState(ch, current_s);

    // Very weird logic i dont know why i did this
    if(nextState == TRAP_STATE){
        if(pos>=1){
            retract(1);
        }
        else{
            token->lexeme[pos] = ch;
            token->lexeme[++pos] = '\0';
        }
        return NOTINVALID;
    }
    
    int stateDetail = getStateDetails(nextState);

    // only add to lexeme if not in final retract state
    if(stateDetail != FINAL_RETRACTONCE && stateDetail != FINAL_RETRACTTWICE){
        token->lexeme[pos] = ch;
        token->lexeme[++pos] = '\0';
       
    }

    if(stateDetail != NON_FINAL){

        if (pos >= LENGTHLEXEME)
        {
            return LENGTHEXCEEDED;
        }

        int do_state_actions = doStateActions(token, nextState);
        // printf("Line no:%d sd=%d , type = %s , STATE : %d, l = %s\n", lineNo, stateDetail, getTokenStr(token->type), nextState, token->lexeme);
        return do_state_actions;
        
    }
    
    else
    {   
        current_s = nextState;
        return newGetToken(token, pos);
    }
}

int printToken(Token* t)
{

    if (t->type == TK_EOF)
    {
        return -1;
    }
    if (t->type == TK_ERR)
    {
        printf("Line no. %d\t Error: Unknown pattern <%s> \n", t->lineNo, t->lexeme);
    }
    else
    {
        const char *tokenName = NULL;
        tokenName = getTokenStr(t->type);
        printf("Line no. %d\t Lexeme %-10s\t Token %s\n", t->lineNo, t->lexeme, tokenName);
    }
    return 1;
}

void driverToken(FILE *fp){

    initLexer(fp);

    while (1)
    {
        Token t;
        t.lexeme[0] = '\0';
        t.lineNo = lineNo;
        t.type = TK_ERR;

        int x = newGetToken(&t, 0);

        current_s = 0;
        if (x == EXIT)
        {
            break;
        }
        if(x == CONTINUE){
            continue;
        }
        
        printToken(&t);
        
    }

    closeLexer();
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
        fp = stdin;
    }

    driverToken(fp);
    
    return 0;
}
