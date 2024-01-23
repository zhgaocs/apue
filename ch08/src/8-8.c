/**
 * exec function example
 */
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

char *env_init[] = {"USER=unknown", "PATH=/tmp", NULL};

int main()
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid == 0) /* specify pathname, specify environment */
        if (execle("/home/sar/bin/echoall", "echoall",
                   "myarg1", "MY ARG2", (char *)0, env_init) < 0)
        {
            perror("execle error");
            exit(1);
        }

    if (waitpid(pid, NULL, 0) < 0)
    {
        perror("wait error");
        exit(1);
    }

    if ((pid = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid == 0) /* specify filename, inherit environment */
        if (execlp("echoall", "echoall", "only 1 arg", (char *)0) < 0)
        {
            perror("execlp error");
            exit(1);
        }

    exit(0);
}
