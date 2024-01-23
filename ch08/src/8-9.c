/**
 * echo all command line arguments and all environment strings
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int i;
    char **ptr;
    extern char **environ;

    for (i = 0; i < argc; i++)
        printf("argv[%d]: %s\n", i, argv[i]);

    for (ptr = environ; *ptr != 0; ptr++)
        printf("%s\n", *ptr);

    exit(0);
}
