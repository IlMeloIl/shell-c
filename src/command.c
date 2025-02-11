#include "command.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int handle_executable(char **tokens) {
    char *cmd_path = find_executable(tokens[0]);
    if (!cmd_path)
        return 1;
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        free(cmd_path);
        return 1;
    }
    if (pid == 0) {
        if (execvp(tokens[0], tokens) == -1) {
            perror("execvp");
            exit(1);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
    free(cmd_path);
    return 0;
}
