/**
 * umask function example
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main()
{
    umask(0);

    if (creat("foo", RWRWRW) < 0)
    {
        fprintf(stderr, "creat error for foo");
        exit(1);
    }

    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (creat("bar", RWRWRW) < 0)
    {
        fprintf(stderr, "creat error for bar");
        exit(1);
    }

    exit(0);
}
