#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define FILE_MODE 0666

int main(int argc, char *argv[])
{
    int fdin, fdout;
    void *src, *dst;
    struct stat statbuf;

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <fromfile> <tofile>", argv[0]);
        exit(1);
    }

    if ((fdin = open(argv[1], O_RDONLY)) < 0)
    {
        fprintf(stderr, "can't open %s for reading", argv[1]);
        exit(1);
    }

    if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
    {
        fprintf(stderr, "can't open %s for writing", argv[2]);
        exit(1);
    }

    if (fstat(fdin, &statbuf) < 0) /* need size of input file */
    {
        perror("fstat error");
        exit(1);
    }

    /* set size of output file */
    if (lseek(fdout, statbuf.st_size - 1, SEEK_SET) == -1)
    {
        perror("lseek error");
        exit(1);
    }

    if (write(fdout, "", 1) != 1)
    {
        perror("write error");
        exit(1);
    }

    if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == MAP_FAILED)
    {
        perror("mmap error for input");
        exit(1);
    }

    if ((dst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0)) == MAP_FAILED)
    {
        perror("mmap error for output");
        exit(1);
    }

    memcpy(dst, src, statbuf.st_size); /* does the file copy */
    exit(0);
}