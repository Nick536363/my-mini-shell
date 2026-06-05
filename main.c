#include "mylib.c"

int main(void)
{
    int argc = 0;
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
        argc = 0;
        for(char* space_ptr = cmd; space_ptr != NULL; space_ptr = strchr(space_ptr+1, ' ')){
            if(strchr(space_ptr+1, ' ') != NULL) strncpy(args[argc], space_ptr, strchr(space_ptr+1, ' ') - space_ptr);
            else strncpy(args[argc], space_ptr, cmd != space_ptr ? space_ptr - cmd : strlen(cmd)-1);
            puts(args[argc]);
            argc++;
        }
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