#include "mylib.h"

int main(void)
{
    int argc;
    int cmdc;
    char path[MAX_STR] = START_PATH;
    char cmd[CMD_MAX] = {};
    char args_str[MAX_ARGS][MAX_STR] = {};
    char cmds_str[MAX_ARGS][MAX_STR] = {};
    char* args[MAX_ARGS] = {};
    char* cmds[MAX_ARGS] = {};
    int child_ret_status;
    pid_t cmd_pid;
    int bg;
    int is_pipe;
    int redirected;

    for(int current_cmd = 0; current_cmd < MAX_ARGS; current_cmd++)
            cmds[current_cmd] = cmds_str[current_cmd];
    chdir(path);

    while(1){
        printf("%s λ ", path);
        memset(args_str, 0, sizeof(args_str));
        memset(cmds_str, 0, sizeof(cmds_str));
        memset(cmd, 0, sizeof(cmd));
        fgets(cmd, CMD_MAX, stdin);
        if(cmd[0] == '\n'){
            continue;
        }
        is_pipe = check_if_pipe(cmd);
        redirected = check_if_redirect(cmd);
        if(is_pipe){
            cmdc = parse_cmds(cmd, cmds);
            pipe_exec(cmds, args, args_str, cmdc);
            continue;
        }
        if(redirected){
            cmdc = parse_redirect(cmd, redirected, cmds);
            parse_args(cmds[0], args, args_str);
            cmd_pid = fork();
            if(cmd_pid == -1){
                perror("Creating child procces");
                continue;
            }
            if(cmd_pid == 0){
                redirect(cmds[cmdc-1], redirected);
                execvp(args[0], args);
            }
            wait(&child_ret_status);
            continue;
        }
        argc = parse_args(cmd, args, args_str);
        bg = check_if_bg(args, argc);
        if(strcmp("cd", args[0]) == 0){
            if(chdir(args[1])){
                perror("Changing directory");
                continue;
            }
            if(getcwd(path, MAX_PATH) == NULL){
                perror("getcwd");
            }
            continue;
        }
        cmd_pid = fork();
        if(cmd_pid == -1){
            perror(args[0]);
            continue;
        }
        if(cmd_pid != 0){
            if(!bg)
                waitpid(cmd_pid, &child_ret_status, 0);
            else
                printf("[%d] %s - background\n", cmd_pid, args[0]);
            continue;
        }
        if(execvp(args[0], args) == -1){
            perror(args[0]);
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}