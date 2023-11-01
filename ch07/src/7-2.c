/**
 * terminate handler instance
 */

#include <stdio.h>
#include <stdlib.h>

static void my_exit1();
static void my_exit2();

int main()
{
    if (atexit(my_exit2) != 0)
    {
        fprintf(stderr, "can't register my_exit2");
        exit(1);
    }
    if (atexit(my_exit1) != 0)
    {
        fprintf(stderr, "can't register my_exit1");
        exit(1);
    }

    printf("main is done\n");

    return 0;
}

static void my_exit1()
{
    printf("first exit handler\n");
}

static void my_exit2()
{
    printf("second exit handler\n");
}