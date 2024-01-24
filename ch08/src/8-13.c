/**
 * call system function
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int my_system(const char *);
void pr_exit(int);

int main()
{
    int status;

    if ((status = my_system("date")) < 0)
    {
        perror("my_system() error");
        exit(1);
    }
    pr_exit(status);

    if ((status = my_system("nosuchcommand")) < 0)
    {
        perror("my_system() error");
        exit(1);
    }
    pr_exit(status);

    if ((status = my_system("who; exit 44")) < 0)
    {
        perror("my_system() error");
        exit(1);
    }
    pr_exit(status);

    exit(0);
}

int my_system(const char *cmdstring) /* version without signal handling */
{
    pid_t pid;
    int status;

    if (NULL == cmdstring)
        return 1; /* always a command processor with UNIX */

    if ((pid = fork()) < 0)
        status = -1;   /* probably out of process */
    else if (pid == 0) /* child */
    {
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127); /* execl error */
    }
    else /* parent */
    {
        while (waitpid(pid, &status, 0) < 0)
            if (errno != EINTR)
            {
                status = -1; /* error other than EINTR from waitpid() */
                break;
            }
    }

    return status;
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
