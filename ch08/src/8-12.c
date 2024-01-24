/**
 * system function (no signal processing)
 */
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

int my_system(const char *cmdstring) /* version without signal handling */
{
    pid_t pid;
    int status;

    if (cmdstring == NULL)
        return 1; /* always a command processor with UNIX */

    if ((pid = fork()) < 0)
        status = -1;   /* probably out of process */
    else if (pid == 0) /* child */
    {
        execl("/bin/sh", "sh", "-c", "cmdstring", (char *)0);
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
