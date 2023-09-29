/**
 * setjmp and longjmp examples
 */
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 10
#define TOK_ADD 5

jmp_buf jmp_buffer;

void do_line(char *);
void cmd_add();
int get_token();

int main()
{
    char line[MAXLINE];

    if (setjmp(jmp_buffer) != 0)
        printf("error");

    while (fgets(line, MAXLINE, stdin) != NULL)
        do_line(line);
    exit(0);
}

char *tok_ptr; /* global pointer for get_token */

void do_line(char *ptr) /* process one line of input */
{
    int cmd;

    tok_ptr = ptr;
    while ((cmd = get_token()) > 0)
        switch (cmd)
        {
        case TOK_ADD:
            cmd_add();
            break;
        }
}

void cmd_add()
{
    int token;

    token = get_token();
    if (token < 0) /* an error has occurred */
        longjmp(jmp_buffer, 1);

    /* rest of processing for this command */
}

int get_token()
{
    /* fetch next token from line pointed to by tok_ptr */
}