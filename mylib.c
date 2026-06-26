#ifndef __IS_LIB__
#define __IS_LIB__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include "mylib.h"


int parse_args(char* cmd, char* args_ptrs[], char args_orig[][MAX_STR])
{   
    for(int arg = 0; arg < MAX_ARGS; arg++)
            args_ptrs[arg] = args_orig[arg];

    int argc = 0;
    char* arg = strtok(cmd, " \n");
    if(arg == NULL)
        strcpy(args_ptrs[argc++], cmd);
    else
        while(arg != NULL){
            strcpy(args_ptrs[argc++], arg);
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
    if(strchr(cmd, '|') != NULL)
        return 1;
    return 0;
}


int parse_cmds(char* cmd, char* cmds[])
{
    int cmdc = 0;
    char* current_cmd = strtok(cmd, "|");

    if(current_cmd == NULL)
        strcpy(cmds[cmdc++], current_cmd);
    else
        while(current_cmd != NULL){
            strcpy(cmds[cmdc++], current_cmd);
            current_cmd = strtok(NULL, "|\n");
        }
    
    return cmdc;
}


int pipe_exec(char* cmds[], char* args[], char args_str[][MAX_STR], int cmdc){
    int fd[cmdc-1][2];
    int return_status;
    pid_t cmds_pid[cmdc];
    for(int pipec = 0; pipec < cmdc-1; pipec++)
        if(pipe(fd[pipec]) == -1){
            perror("Creating pipe");
            exit(EXIT_FAILURE);
        }
    for(int cmd = 0; cmd < cmdc; cmd++){
        parse_args(cmds[cmd], args, args_str);
        
        cmds_pid[cmd] = fork();
        if(cmds_pid[cmd] == -1){
            perror("Creating child");
            exit(EXIT_FAILURE);
        }
        
        if(cmds_pid[cmd] == 0){
            if(cmd == 0){
                if(dup2(fd[cmd][1], 1) == -1){
                    perror("Duplicate file descriptor");
                    exit(EXIT_FAILURE);
                }
            }
            
            else if(cmd < cmdc-1){
                if(dup2(fd[cmd-1][0], 0) == -1 || dup2(fd[cmd][1], 1) == -1){
                    perror("Duplicate file descriptor");
                    exit(EXIT_FAILURE);
                }
            }

            else{
                if(dup2(fd[cmd-1][0], 0) == -1){
                    perror("Duplicate file descriptor");
                    exit(EXIT_FAILURE);
                }
            }

            for(int pipec = 0; pipec < cmdc-1; pipec++)
                if(close(fd[pipec][0]) == -1 || close(fd[pipec][1]) == -1){
                    perror("Closing pipe");
                    exit(EXIT_FAILURE);
                }

            if(execvp(args[0], args) == -1){
                perror(args[0]);
                exit(EXIT_FAILURE);
            }

        }
    }

    for(int pipec = 0; pipec < cmdc-1; pipec++){
        if(close(fd[pipec][0]) == -1 || close(fd[pipec][1]) == -1){
            perror("Closing pipe");
            exit(EXIT_FAILURE);
        }
    }
    for(int cmd = 0; cmd < cmdc; cmd++){
        if(waitpid(cmds_pid[cmd], &return_status, 0) == -1){
            perror("Waiting for child procces");
            exit(EXIT_FAILURE);
        }
    }
}


int check_if_redirect(char *cmd)
{
    if(strchr(cmd, '<') != NULL)
        return 1;
    if(strchr(cmd, '>') != NULL)
        return 2;
    return 0;
}


int parse_redirect(char* cmd, int mode, char* dst[]){
    char* token = mode == 1 ? " < \n" : " > \n";
    int cmdc = 0;
    char* current_cmd = strtok(cmd, token);

    while(current_cmd != NULL){
        strcpy(dst[cmdc++], current_cmd);
        current_cmd = strtok(NULL, token);
    }
    
    return cmdc;

}


void redirect(char* filepath, int mode)
{
    int fd;
    switch(mode){
        case 1:
            fd = open(filepath, O_RDONLY);
            if(fd == -1){
                perror("Opening file");
                exit(EXIT_FAILURE);
            }
            if(dup2(fd, 0) == -1){
                perror("Redirecting data");
                exit(EXIT_FAILURE);
            }
            break;
        case 2:
            fd = open(filepath, O_WRONLY | O_CREAT);
            if(fd == -1){
                perror("Opening file");
                exit(EXIT_FAILURE);
            }
            if(dup2(fd, 1) == -1){
                perror("Redirecting data");
                exit(EXIT_FAILURE);
            }
            break;
    }
}
#endif