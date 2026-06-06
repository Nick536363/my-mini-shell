#include "mylib.c"

int main(void)
{
    int argc;
    char cmd[CMD_MAX] = {};
    char args_str[MAX_ARGS][MAX_STR] = {};
    char* args[MAX_ARGS] = {};
    int child_ret_status;
    pid_t cmd_pid;

    for(int arg = 0; arg < MAX_ARGS; arg++)
            args[arg] = args_str[arg];

    while(1){
        printf("$ ");
        memset(args_str, 0, sizeof(args_str));
        memset(cmd, 0, sizeof(cmd));
        fgets(cmd, CMD_MAX, stdin);
        argc = parse_args(cmd, args);
        cmd_pid = fork();
        if(cmd_pid == -1){
            perror(args[0]);
            continue;
        }
        if(cmd_pid != 0){
            waitpid(cmd_pid, &child_ret_status, 0);
            continue;
        }
        if(execvp(args[0], args) == -1){
            perror(args[0]);
        }
    }
    return 0;
}