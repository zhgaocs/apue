/* 
 *  copy standard input to standard output
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFSIZE 4096

int main()
{
    int n;
    char buf[BUFFSIZE];

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n)
            fprintf(stderr, "write error");

    if (n < 0)
        fprintf(stderr, "write error");

    exit(0);
}