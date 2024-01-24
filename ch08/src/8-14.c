/**
 * use system to execute command line parameters
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void pr_exit(int);

int main(int argc, char *argv[])
{
    int status;

    if (argc < 2)
    {
        fprintf(stderr, "command-line argument required\n");
        exit(1);
    }

    if ((status = system(argv[1])) < 0)
    {
        perror("system() error");
        exit(1);
    }
    pr_exit(status);

    exit(0);
}

void pr_exit(int status)
{
    if (WIFEXITED(status))
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));

    else if (WIFSIGNALED(status))
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),

#ifdef WCOREDUMP
               WCOREDUMP(status) ? " (core file generated)" : "");
#else
               "");
#endif
    else if (WIFSTOPPED(status))
        printf("child stopped, signal number = %d\n", WSTOPSIG(status));
}
