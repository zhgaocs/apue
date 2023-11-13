/**
 * copy standard input to standard output using fgets and fputs
 */
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 16

int main()
{
    char buf[MAXLINE];

    while (fgets(buf, MAXLINE, stdin) != NULL)
        if (fputs(buf, stdout) == EOF)
        {
            fprintf(stderr, "output error");
            exit(1);
        }

    if (ferror(stdin))
    {
        fprintf(stderr, "input error");
        exit(1);
    }

    exit(0);
}