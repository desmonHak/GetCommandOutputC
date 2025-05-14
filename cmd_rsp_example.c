#include <stdio.h>         // printf() and NULL
#include <stdlib.h>        // EXIT_FAILURE, EXIT_SUCCESS and NULL

#include "cmd_rsp.h"

// https://codereview.stackexchange.com/questions/162546/send-command-and-get-response-from-windows-cmd-prompt-silently

#define MY_BUFFER_SIZE    100

int main(void)
{
    int status = EXIT_SUCCESS;
    char *buf = NULL;
    buf = calloc(MY_BUFFER_SIZE, 1);
    if(!buf)return 0;
    if (!cmd_rsp("dir /s", &buf, MY_BUFFER_SIZE))
    {
        printf("%s", buf);
    }
    else
    {
        status = EXIT_FAILURE;
    }
    free(buf);

    return status;
}
