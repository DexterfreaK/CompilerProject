// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define BUFFER_SIZE 8 // can be any convenient size
// #define EOF_SENTINEL '\0'

// // Global/static variables to keep it simple
// static FILE *source = NULL;
// static char buffer[2][BUFFER_SIZE + 1]; // Two buffers, each with space for a sentinel
// static int currentBuffer = 0;           // Which buffer is active? 0 or 1
// static int forwardPointer = 0;          // Index within the active buffer
// static int charsInBuffer[2] = {0, 0};   // How many valid chars were loaded in each buffer?

// //--------------------------------------------------------------
// // Loads data into the specified buffer from the file.
// // Returns the number of characters actually read.
// //--------------------------------------------------------------
// int loadBuffer(int whichBuffer)
// {
//     if (!source)
//     {
//         return 0;
//     }

//     // Read up to BUFFER_SIZE characters
//     size_t bytesRead = fread(buffer[whichBuffer], sizeof(char), BUFFER_SIZE, source);
//     buffer[whichBuffer][bytesRead] = EOF_SENTINEL; // sentinel or EOF marker

//     charsInBuffer[whichBuffer] = (int)bytesRead;

//     return (int)bytesRead;
// }

// //--------------------------------------------------------------
// // Initialize the twin buffer system, given an already opened FILE*.
// // Loads the first buffer, sets pointers to start.
// //--------------------------------------------------------------
// void initTwinBuffer(FILE *fp)
// {
//     source = fp;
//     if (!source)
//     {
//         fprintf(stderr, "Source file pointer is NULL!\n");
//         exit(EXIT_FAILURE);
//     }

//     // Load the first buffer
//     currentBuffer = 0;
//     forwardPointer = 0;

//     int readCount = loadBuffer(currentBuffer);
//     if (readCount == 0)
//     {
//         // No data or file is empty
//         // We'll rely on the sentinel in the buffer for getNextChar
//     }
// }

// //--------------------------------------------------------------
// // Closes the source file (if we need to).
// //--------------------------------------------------------------
// void closeTwinBuffer()
// {
//     if (source)
//     {
//         fclose(source);
//         source = NULL;
//     }
// }

// //--------------------------------------------------------------
// // Switches to the other buffer (used when we hit the sentinel or reach end).
// //--------------------------------------------------------------
// static void switchBuffer()
// {
//     // Switch from buffer 0 -> 1 or buffer 1 -> 0
//     currentBuffer = 1 - currentBuffer;
//     forwardPointer = 0;

//     // Load the newly active buffer
//     int readCount = loadBuffer(currentBuffer);
//     // If readCount = 0, we have EOF sentinel set
// }

// //--------------------------------------------------------------
// // Get the next character from the twin-buffer system.
// //--------------------------------------------------------------
// char getNextChar()
// {
//     char ch = buffer[currentBuffer][forwardPointer];

//     if (ch == EOF_SENTINEL)
//     {
//         // We hit the sentinel in our current buffer
//         // Check if we are truly at EOF or just need to switch buffers

//         // If we have read fewer than BUFFER_SIZE last time,
//         // this sentinel means actual EOF.
//         if (charsInBuffer[currentBuffer] < BUFFER_SIZE)
//         {
//             // True EOF
//             return EOF;
//         }
//         else
//         {
//             // Switch to other buffer
//             switchBuffer();
//             ch = buffer[currentBuffer][forwardPointer];
//         }
//     }

//     // If still EOF after switch, we are truly done
//     if (ch == EOF_SENTINEL)
//     {
//         return EOF;
//     }

//     // Move forward pointer
//     forwardPointer++;
//     return ch;
// }

// //--------------------------------------------------------------
// // Retract the forward pointer by n (1 or 2).
// // We must handle crossing the buffer boundary if needed.
// //--------------------------------------------------------------
// void retract(int n)
// {
//     // Move the forward pointer back by n in the current buffer if possible
//     forwardPointer -= n;
//     if (forwardPointer < 0)
//     {
//         // We have crossed into the other buffer
//         // Switch back to the other buffer
//         currentBuffer = 1 - currentBuffer;

//         // The new forward pointer should point near the end of that buffer
//         // but we have to handle the sentinel logic carefully:
//         forwardPointer = charsInBuffer[currentBuffer] + forwardPointer;
//         if (forwardPointer < 0)
//         {
//             // If somehow still negative, it means we tried to retract too far
//             // beyond the twin-buffer design. For a robust lexer, you might
//             // handle an error or keep more history.
//             fprintf(stderr, "Error: Retraction too large!\n");
//             forwardPointer = 0;
//         }
//     }
// }

// //--------------------------------------------------------------
// // A simple main to test reading and retracting characters.
// //--------------------------------------------------------------
// int main(int argc, char *argv[])
// {
//     // For demonstration, we'll open a file if given, else use stdin
//     FILE *fp = NULL;
//     if (argc > 1)
//     {
//         fp = fopen(argv[1], "r");
//         if (!fp)
//         {
//             perror("Failed to open file");
//             return 1;
//         }
//     }
//     else
//     {
//         fp = stdin;
//     }

//     // Initialize the twin buffer
//     initTwinBuffer(fp);

//     printf("Reading characters with twin-buffer system.\n");

//     // Get some characters
//     char c;
//     int f = 0;
//     while ((c = getNextChar()) != EOF)
//     {
//         putchar(c);
//     }

//     printf("\nEnd of file reached.\n");

//     // Cleanup
//     closeTwinBuffer();
//     return 0;
// }
