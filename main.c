#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(void){
    const char* bin = "/bin/";
    char cmd[20] = {};
    char* args[20] = {};
    while(1){
        printf("> ");
        scanf("%s", cmd);
        if(execvp(cmd, args) == -1)
            perror(cmd);
    }
    return 0;
}