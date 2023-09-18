/**
 * create a file with holes
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main()
{
    int fd;

    if ((fd = creat("file.hole", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
        fprintf(stderr, "creat error");

    if (write(fd, buf1, 10) != 10)
        fprintf(stderr, "buf1 write error");
    /* offset now = 10 */

    if (lseek(fd, 16384, SEEK_SET) == -1)
        fprintf(stderr, "lseek error");
    /* offset now = 16384 */

    if (write(fd, buf2, 10) != 10)
        fprintf(stderr, "buf2 write error");
    /* offset now = 16394 */

    exit(0);
}