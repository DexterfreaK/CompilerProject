#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "parser.h"


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    while (1)
    {
        int input;
        printf("Enter 0 to exit, 1 to remove comments, 2 to print tokens, 3 to get parse, 4 to get time\n");
        scanf("%d", &input);

        switch (input)
        {
        case 0:
            printf("Exit program\n");
            return EXIT_SUCCESS;
            break;
        case 1:
            removeComments(argv[1]);
            break;
        case 2:
            driverToken(argv[1]);
            break;
        case 3:
            parser_main();
            break;
        case 4:
        {
            clock_t start_time, end_time;
            double total_CPU_time, total_CPU_time_in_seconds;
            start_time = clock();
            driverToken(argv[1]); // replace driverToken(argv[1]); with the parser function
            end_time = clock();

            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

            // Print both total_CPU_time and total_CPU_time_in_seconds
            printf("\nTotal CPU time for Lexer and Parser is: %f\n", total_CPU_time);
            printf("Total CPU time in seconds for Lexer and Parser: %f\n\n", total_CPU_time_in_seconds);
        }
        break;

        default:
            printf("Exit program\n");
            return EXIT_SUCCESS;
            break;
        }
    }
}
