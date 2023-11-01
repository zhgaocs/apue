/**
 * is it possible to set an offset to standard input
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
        printf("cannot seek\n");
    else
        printf("seek ok\n");

    exit(0);
}
