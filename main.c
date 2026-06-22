#include "mylib.c"

int main(void)
{
    int argc;
    int cmdc;
    char* path = START_PATH;
    char cmd[CMD_MAX] = {};
    char args_str[MAX_ARGS][MAX_STR] = {};
    char cmds_str[MAX_ARGS][MAX_STR] = {};
    char* args[MAX_ARGS] = {};
    char* cmds[MAX_ARGS] = {};
    int child_ret_status;
    pid_t cmd_pid;
    int bg;
    int is_pipe;
    
    for(int current_cmd = 0; current_cmd < MAX_ARGS; current_cmd++)
            cmds[current_cmd] = cmds_str[current_cmd];
    chdir(path);

    while(1){
        printf("%s $ ", path);
        memset(args_str, 0, sizeof(args_str));
        memset(cmds_str, 0, sizeof(cmds_str));
        memset(cmd, 0, sizeof(cmd));
        fgets(cmd, CMD_MAX, stdin);
        if(cmd[0] == '\n'){
            continue;
        }
        int is_pipe = check_if_pipe(cmd);
        if(is_pipe){
            cmdc = parse_cmds(cmd, cmds);
            pipe_exec(cmds, args, args_str, cmdc, &cmd_pid);
            continue;
        }
        argc = parse_args(cmd, args, args_str);
        bg = check_if_bg(args, argc);
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
        if(strcmp("chdir", args[0]) == 0){
            chdir(args[1]);
            path = strdup(args[1]);
            continue;
        }
        if(strcmp("exit", args[0]) == 0){
            free(path);
            exit(EXIT_SUCCESS);
        }
        if(execvp(args[0], args) == -1){
            perror(args[0]);
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}