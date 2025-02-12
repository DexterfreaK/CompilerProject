#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    driverToken(argv[1]);
    removeComments(argv[1]);
    
    return EXIT_SUCCESS;
}
