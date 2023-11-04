/**
 * getcwd function example
 */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *path_alloc(int *size);

int main()
{
    char *ptr;
    int size = 32;

    if (chdir(".") < 0)
    {
        fprintf(stderr, "chdir failed");
        exit(1);
    }

    ptr = path_alloc(&size);

    if (NULL == getcwd(ptr, size))
    {
        fprintf(stderr, "getcwd failed");
        exit(1);
    }

    printf("cwd = %s\n", ptr);
    exit(0);
}

char *path_alloc(int *sizeptr)
{
    char *ptr;

    if (*sizeptr > PATH_MAX)
    {
        fprintf(stderr, "size too big");
        exit(1);
    }

    *sizeptr = PATH_MAX;

    if ((ptr = malloc(*sizeptr)) == NULL)
    {
        fprintf(stderr, "malloc error for pathname");
        exit(1);
    }

    return ptr;
}