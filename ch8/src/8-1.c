/**
 * example of fork function
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int glob = 6; /* external variable in initialized data */
char buf[] = "a write to stdout\n";

int main()
{
    int var; /* automatic variable on the stack */
    pid_t pid;

    var = 88;

    if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1)
    {
        fprintf(stderr, "write error");
        exit(1);
    }

    printf("before fork\n");

    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error");
        exit(1);
    }
    else if (pid == 0) /* child */
    {
        glob++; /* modify variables */
        var++;
    }
    else
        sleep(2); /* parent */

    printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
    exit(0);
}
