#ifndef __INCLUDED__
#define __INCLUDED__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_ARGS    10
#define MAX_STR     20
#define CMD_MAX     MAX_ARGS * MAX_STR


int parse_args(char* cmd, char* args[])
{
    int argc = 0;
    char* arg = strtok(cmd, " \n");
    if(arg == NULL)
        strncpy(args[argc++], cmd, strlen(cmd));
    else
        while(arg != NULL){
            strncpy(args[argc++], arg, strlen(arg));
                arg = strtok(NULL, " \n");
        }
    args[argc] = NULL;
    return argc;
}


#endif 