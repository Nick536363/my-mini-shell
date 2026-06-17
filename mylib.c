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


int check_if_bg(char* args[], int argc)
{
    if(!strncmp(args[argc-1], "%", strlen(args[argc-1]))){
        args[argc-1] = NULL;
        return 1;
    }
    if(args[argc-1][strlen(args[argc-1])-1] == '%'){
        args[argc-1][strlen(args[argc-1])-1] = '\0';
        return 1;
    }

    return 0;
}


int check_if_pipe(char* cmd)
{
    char* pipe_symb = strchr(cmd, '|');
    if(pipe_symb != NULL)
        return 1;
    return 0;
}


int parse_cmds(char* cmd, char* cmds[])
{
    int cmdc = 0;
    char* current_cmd = strtok(cmd, "|");
    if(current_cmd == NULL)
        strncpy(cmds[cmdc++], current_cmd, strlen(current_cmd));
    else
        while(current_cmd != NULL){
            strncpy(cmds[cmdc++], current_cmd, strlen(current_cmd));
                current_cmd = strtok(NULL, "|\n");
        }
    cmds[cmdc] = NULL;
    return cmdc;
}
#endif 