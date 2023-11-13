/**
 * tmpnam and tmpfile function examples
 */
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 50

int main()
{
    char name[L_tmpnam], line[MAXLINE];
    FILE *fp;

    printf("%s\n", tmpnam(NULL)); /* first temp name */
    tmpnam(name);                 /* second temp name */
    printf("%s\n", name);

    if ((fp = tmpfile()) == NULL) /* create temp file */
    {
        fprintf(stderr, "tmpfile error");
        exit(1);
    }

    fputs("one line of output\n", fp); /* write to temp file */
    rewind(fp);                        /* then read it back */
    if (fgets(line, sizeof(line), fp) == NULL)
    {
        fprintf(stderr, "fgets error");
        exit(1);
    }

    fputs(line, stdout);

    exit(0);
}