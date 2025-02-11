#include "builtin.h"
#include "util.h"  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int builtin_exit(char **tokens, int token_count) {
    if (token_count >= 2 && strcmp(tokens[1], "0") == 0)
        return 0; 
    return 1;
}

int builtin_echo(char **tokens, int token_count) {
    for (int i = 1; i < token_count; i++) {
        printf("%s", tokens[i]);
        if (i < token_count - 1)
            printf(" ");
    }
    printf("\n");
    return 0;
}

int builtin_type(char **tokens, int token_count) {
    if (token_count < 2) {
        printf("type not found\n");
        return 0;
    }
    const char *arg = tokens[1];
    const char *builtins_list[] = {"exit", "echo", "type", "pwd", "cd"};
    int num_builtin_list = sizeof(builtins_list) / sizeof(builtins_list[0]);
    for (int i = 0; i < num_builtin_list; i++) {
        if (strcmp(arg, builtins_list[i]) == 0) {
            printf("%s is a shell builtin\n", arg);
            return 0;
        }
    }
    char *executable = find_executable(arg);
    if (executable) {
        printf("%s is %s\n", arg, executable);
        free(executable);
    } else {
        printf("%s: not found\n", arg);
    }
    return 0;
}

int builtin_pwd(char **tokens, int token_count) {
    (void)tokens; (void)token_count;
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("getcwd");
    return 0;
}

int builtin_cd(char **tokens, int token_count) {
    const char *dir = NULL;
    if (token_count < 2) {
        dir = getenv("HOME");
        if (dir == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return 0;
        }
    } else {
        dir = tokens[1];
        if (strcmp(dir, "~") == 0) {
            dir = getenv("HOME");
            if (dir == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
                return 0;
            }
        }
    }
    if (chdir(dir) == -1) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "cd: %s", dir);
        perror(buffer);
    }
    return 0;
}

Builtin builtins[] = {
    {"exit", builtin_exit},
    {"echo", builtin_echo},
    {"type", builtin_type},
    {"pwd", builtin_pwd},
    {"cd", builtin_cd}
};

int num_builtins = sizeof(builtins) / sizeof(builtins[0]);
