/**
 * open a file and unlink it
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    if (open("tempfile", O_RDWR) < 0)
    {
        fprintf(stderr, "open error for tempfile");
        exit(1);
    }

    if (unlink("tempfile") < 0)
    {
        fprintf(stderr, "unlink error");
        exit(1);
    }

    printf("tempfile unlinked\n");
    sleep(15);
    printf("done\n");
    exit(0);
}