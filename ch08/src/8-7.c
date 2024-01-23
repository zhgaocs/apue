/**
 * modify Listing 8-6 to avoid race conditions
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static sig_atomic_t sigflag;
static sigset_t newmask, oldmask, zeromask;

static void charatatime(char *);
static void sig_handler(int);
void TELL_WAIT();
void WAIT_PARENT(void);
void TELL_CHILD(pid_t);

int main()
{
    pid_t pid;
    TELL_WAIT();

    if ((pid = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid == 0)
    {
        WAIT_PARENT();
        charatatime("output from child\n");
    }
    else
    {
        charatatime("output from parent\n");
        TELL_CHILD(pid);
    }

    exit(0);
}

static void charatatime(char *str)
{
    char *ptr;
    int c;

    setbuf(stdout, NULL);
    for (ptr = str; (c = *ptr++) != 0;)
        putc(c, stdout);
}

static void sig_handler(int signo)
{
    sigflag = 1;
}

void TELL_WAIT(void)
{
    if (signal(SIGUSR1, sig_handler) == SIG_ERR)
        perror("signal (SIGUSR1) error");
    if (signal(SIGUSR2, sig_handler) == SIG_ERR)
        perror("signal (SIGUSR2) error");

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        perror("SIG_BLOCK error");
}

void WAIT_PARENT(void)
{
    while (sigflag == 0)
        sigsuspend(&zeromask);
    sigflag = 0;

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        perror("SIG_SETMASK error");
}

void TELL_CHILD(pid_t pid)
{
    kill(pid, SIGUSR1);
}
