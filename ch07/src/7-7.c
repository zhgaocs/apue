/**
 * incorrect use of automatic variables
 */
#include <stdio.h>

#define DATAFILE "datafile"

FILE *open_data()
{
    FILE *fp;
    char databuf[BUFSIZ]; /* setvbuf makes this the stdio buffer */

    if ((fp = fopen(DATAFILE, "r")) == NULL)
        return NULL;
    if (setvbuf(fp, databuf, _IOLBF, BUFSIZ) != 0)
        return NULL;

    return fp; /* error */
}