/**
 * open one or more file status flags for a file descriptor
 */
#include <fcntl.h>
#include <stdio.h>

void set_fl(int fd, int flags) // flags are file status flags to turn on
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        fprintf(stderr, "fcntl F_GETFL error");

    val |= flags; // turn on flags

    if (fnctl(fd, F_SETFL, val) < 0)
        fprintf(stderr, "fcntl F_SETFL error");
}