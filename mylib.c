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
#define START_PATH "/home/nickf"

int parse_args(char* cmd, char* args_ptrs[], char args_orig[][MAX_STR])
{   
    for(int arg = 0; arg < MAX_ARGS; arg++)
            args_ptrs[arg] = args_orig[arg];

    int argc = 0;
    char* arg = strtok(cmd, " \n");
    if(arg == NULL)
        strncpy(args_ptrs[argc++], cmd, strlen(cmd));
    else
        while(arg != NULL){
            strncpy(args_ptrs[argc++], arg, strlen(arg));
                arg = strtok(NULL, " \n");
        }
    args_ptrs[argc] = NULL;
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
        strncpy(cmds[cmdc++], current_cmd, strlen(current_cmd)+1);
    else
        while(current_cmd != NULL){
            strncpy(cmds[cmdc++], current_cmd, strlen(current_cmd)+1);
                current_cmd = strtok(NULL, "|\n");
        }
    
    return cmdc;
}


int pipe_exec(char* cmds[], char* args[], char args_str[][MAX_STR], int cmdc, pid_t* cmd_pid){
    int fd[cmdc-1][2];
    int argc;

    for(int pipec = 0; pipec < cmdc-1; pipec++)
        pipe(fd[pipec]);
    for(int cmd = 0; cmd < cmdc; cmd++){
        argc = parse_args(cmds[cmd], args, args_str);
        
        *cmd_pid = fork();
        
        if(*cmd_pid == 0){
            if(cmd == 0){
                dup2(fd[cmd][1], 1);
                close(fd[cmd][1]);
            }
            else if(cmd < cmdc-1){
                dup2(fd[cmd-1][0], 0);
                dup2(fd[cmd][1], 1);
                close(fd[cmd-1][0]);
                close(fd[cmd][1]);
            }
            else{
                dup2(fd[cmd-1][0], 0);
                close(fd[cmd-1][0]);
            }
            execvp(args[0], args);
        }
       // memset(args, 0, MAX_ARGS*MAX_STR*sizeof(char));
    }
}
#endif 