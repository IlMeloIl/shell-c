#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *find_executable(const char *cmd) {
    const char *path = getenv("PATH");
    if (!path)
        return NULL;
    char *path_copy = strdup(path);
    if (!path_copy) {
        perror("strdup failed");
        return NULL;
    }
    char *dir = strtok(path_copy, ":");
    char fullPath[512];
    while (dir != NULL) {
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dir, cmd);
        if (access(fullPath, X_OK) == 0) {
            free(path_copy);
            return strdup(fullPath);
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}
