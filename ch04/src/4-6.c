/**
 * utime function example
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <utime.h>

int main(int argc, char *argv[])
{
    int i, fd;
    struct stat statbuf;
    struct utimbuf timebuf;

    for (i = 1; i < argc; i++)
    {
        if (stat(argv[i], &statbuf) < 0) /* fetch current times */
        {
            fprintf(stderr, "%s: stat error", argv[i]);
            continue;
        }

        if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0) /* truncate */
        {
            fprintf(stderr, "%s: open error", argv[i]);
            continue;
        }

        close(fd);
        timebuf.actime = statbuf.st_atime;
        timebuf.modtime = statbuf.st_mtime;

        if (utime(argv[i], &timebuf) < 0) /* reset times */
        {
            fprintf(stderr, "%s: utime error", argv[i]);
            continue;
        }
    }

    exit(0);
}