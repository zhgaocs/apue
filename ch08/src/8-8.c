/**
 * exec function example
 */
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

char *my_env[] = {"USER=zhgaocs", (char *)0};

int main()
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid == 0) /* specify pathname, specify environment */
        if (execle("/usr/bin/bash", "bash", "-c", "echo $USER", (char *)0, my_env) < 0)
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
        if (execlp("bash", "bash", "-c", " echo hello, world", (char *)0) < 0)
        {
            perror("execlp error");
            exit(1);
        }

    exit(0);
}
