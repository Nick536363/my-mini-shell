#include "mylib.c"

int main(void){
    char cmd[20] = {};
    char* args[20] = {};
    int child_ret_status;
    pid_t cmd_pid;
    while(1){
        printf("> ");
        scanf("%19s", cmd);
        cmd_pid = fork();
        if(cmd_pid == -1){
            perror(cmd);
            continue;
        }
        if(cmd_pid != 0){
            waitpid(cmd_pid, &child_ret_status, 0);
            continue;
        }
        if(execvp(cmd, args) == -1){
            perror(cmd);
        }
    }
    return 0;
}