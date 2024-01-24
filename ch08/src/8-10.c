/**
 * A program that executes an interpreter file
 */
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid == 0) /* child */
    {
        /**
         * touch /root/test.sh
         * chmod +x /root/test.sh
         */
        if (execl("/root/test.sh", "test.sh", (char *)0) < 0)
        {
            perror("execl error");
            exit(1);
        }
    }

    if (waitpid(pid, NULL, 0) < 0) /* parent */
    {
        perror("waitpid error");
        exit(1);
    }

    exit(0);
}
