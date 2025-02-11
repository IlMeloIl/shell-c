#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <wordexp.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include "trie.h"
#include "builtin.h"
#include "util.h"
#include "input.h"
#include "redirection.h"
#include "terminal.h"
#include "command.h"

#ifndef MAX_WORD_LEN
#define MAX_WORD_LEN 100
#endif

int main() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(restore_terminal);
    struct termios new_termios = orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    TrieNode *trie_root = createNode();
    insert(trie_root, "echo");
    insert(trie_root, "exit");
    insert(trie_root, "type");
    insert(trie_root, "pwd");
    insert(trie_root, "cd");

    const char *path = getenv("PATH");
    if (path) {
        char *path_copy = strdup(path);
        if (path_copy) {
            char *dir = strtok(path_copy, ":");
            while (dir != NULL) {
                DIR *d = opendir(dir);
                if (d) {
                    struct dirent *entry;
                    while ((entry = readdir(d)) != NULL) {
                        if (entry->d_name[0] == '.')
                            continue;
                        char fullPath[512];
                        snprintf(fullPath, sizeof(fullPath), "%s/%s", dir, entry->d_name);
                        if (access(fullPath, X_OK) == 0) {
                            if (!is_command_in_trie(trie_root, entry->d_name))
                                insert(trie_root, entry->d_name);
                        }
                    }
                    closedir(d);
                }
                dir = strtok(NULL, ":");
            }
            free(path_copy);
        }
    }

    while (1) {
        char prompt[] = "$ ";
        write(STDOUT_FILENO, prompt, strlen(prompt));

        char input[MAX_WORD_LEN] = {0};
        size_t input_len = 0;
        while (1) {
            char c;
            if (read(STDIN_FILENO, &c, 1) != 1) {
                perror("read");
                exit(1);
            }
            if (c == '\n') {
                write(STDOUT_FILENO, &c, 1);
                break;
            } else if (c == '\t') {
                handle_tab(input, &input_len, trie_root);
                redraw_line(prompt, input);
            } else if (c == 127 || c == '\b') {
                if (input_len > 0) {
                    input_len--;
                    input[input_len] = '\0';
                    redraw_line(prompt, input);
                }
            } else if (isprint(c)) {
                if (input_len < MAX_WORD_LEN - 1) {
                    input[input_len] = c;
                    input_len++;
                    input[input_len] = '\0';
                    redraw_line(prompt, input);
                }
            }
        }

        char *outfile = NULL;
        char *errfile = NULL;
        int append_flag = 0;
        char input_copy[MAX_WORD_LEN];
        strcpy(input_copy, input);
        handle_redirection(input_copy, &outfile, &errfile, &append_flag);

        wordexp_t exp_result;
        if (wordexp(input_copy, &exp_result, 0) != 0) {
            printf("wordexp error\n");
            continue;
        }
        char **tokens = exp_result.we_wordv;
        int token_count = exp_result.we_wordc;

        int saved_stdout = -1;
        int saved_stderr = -1;
        if (outfile != NULL) {
            saved_stdout = dup(STDOUT_FILENO);
            int flags = O_WRONLY | O_CREAT | (append_flag ? O_APPEND : O_TRUNC);
            int fd = open(outfile, flags, 0644);
            if (fd < 0 || dup2(fd, STDOUT_FILENO) < 0) {
                perror("redirection failed");
                close(fd);
                wordfree(&exp_result);
                continue;
            }
            close(fd);
        }
        if (errfile != NULL) {
            saved_stderr = dup(STDERR_FILENO);
            int flags = O_WRONLY | O_CREAT | (append_flag ? O_APPEND : O_TRUNC);
            int fd_err = open(errfile, flags, 0644);
            if (fd_err < 0 || dup2(fd_err, STDERR_FILENO) < 0) {
                perror("error redirection failed");
                close(fd_err);
                wordfree(&exp_result);
                continue;
            }
            close(fd_err);
        }

        int builtin_found = 0;
        for (int i = 0; i < num_builtins; i++) {
            if (strcmp(tokens[0], builtins[i].name) == 0) {
                int ret = builtins[i].handler(tokens, token_count);
                builtin_found = 1;
                if (strcmp(builtins[i].name, "exit") == 0 && ret == 0) {
                    wordfree(&exp_result);
                    exit(0);
                }
                break;
            }
        }
        if (!builtin_found) {
            if (handle_executable(tokens) != 0)
                printf("%s: command not found\n", tokens[0]);
        }
        wordfree(&exp_result);
        if (outfile != NULL || errfile != NULL) {
            if (saved_stdout != -1) {
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdout);
                saved_stdout = -1;
            }
            if (saved_stderr != -1) {
                dup2(saved_stderr, STDERR_FILENO);
                close(saved_stderr);
                saved_stderr = -1;
            }
        }
    }

    freeTrie(trie_root);
    return 0;
}
