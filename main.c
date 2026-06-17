#include "mylib.c"

int main(void)
{
    int argc;
    int cmdc;
    char cmd[CMD_MAX] = {};
    char args_str[MAX_ARGS][MAX_STR] = {};
    char cmds_str[MAX_ARGS][MAX_STR] = {};
    char* args[MAX_ARGS] = {};
    char* cmds[MAX_ARGS] = {};
    int child_ret_status;
    pid_t cmd_pid;
    int bg;
    int is_pipe;
    for(int arg = 0; arg < MAX_ARGS; arg++)
            args[arg] = args_str[arg];
    for(int current_cmd = 0; current_cmd < MAX_ARGS; current_cmd++)
            cmds[current_cmd] = cmds_str[current_cmd];

    while(1){
        printf("$ ");
        memset(args_str, 0, sizeof(args_str));
        memset(cmds_str, 0, sizeof(cmds_str));
        memset(cmd, 0, sizeof(cmd));
        fgets(cmd, CMD_MAX, stdin);
        int is_pipe = check_if_pipe(cmd);
        if(is_pipe){
            cmdc = parse_cmds(cmd, cmds);
            continue;
        }
        argc = parse_args(cmd, args);
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
        if(execvp(args[0], args) == -1){
            perror(args[0]);
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    return 0;
}