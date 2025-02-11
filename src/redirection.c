#include "redirection.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void handle_redirection(char *input, char **outfile, char **errfile, int *append) {
    char *pos = NULL;
    *append = 0;
    if ((pos = strstr(input, "2>>")) != NULL) {
        *append = 1;
        *pos = '\0';
        pos += 3;
        while (*pos == ' ')
            pos++;
        *errfile = pos;
        return;
    }
    if ((pos = strstr(input, "2>")) != NULL) {
        *pos = '\0';
        pos += 2;
        while (*pos == ' ')
            pos++;
        *errfile = pos;
        return;
    }
    if ((pos = strstr(input, "1>>")) != NULL || (pos = strstr(input, ">>")) != NULL) {
        *append = 1;
        char *pos_copy = strdup(pos);
        if (pos_copy == NULL) {
            perror("strdup failed");
            return;
        }
        *pos = '\0';
        pos += (!strncmp(pos_copy, "1>>", 3)) ? 3 : 2;
        while (*pos == ' ')
            pos++;
        *outfile = pos;
        free(pos_copy);
        return;
    }
    if ((pos = strstr(input, "1>")) != NULL || (pos = strstr(input, ">")) != NULL) {
        *append = 0;
        char *pos_copy = strdup(pos);
        if (pos_copy == NULL) {
            perror("strdup failed");
            return;
        }
        *pos = '\0';
        pos += (!strncmp(pos_copy, "1>", 2)) ? 2 : 1;
        while (*pos == ' ')
            pos++;
        *outfile = pos;
        free(pos_copy);
        return;
    }
}
