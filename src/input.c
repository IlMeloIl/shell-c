#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void compute_lcp(char **completions, int count, char *lcp) {
    if (count <= 0) {
        lcp[0] = '\0';
        return;
    }
    strcpy(lcp, completions[0]);
    for (int i = 1; i < count; i++) {
        int j = 0;
        while (lcp[j] && completions[i][j] && lcp[j] == completions[i][j]) {
            j++;
        }
        lcp[j] = '\0';
    }
}

void handle_tab(char *input, size_t *input_len, TrieNode *trie_root) {
    static int tab_press_count = 0;
    static char last_prefix[MAX_WORD_LEN] = "";

    size_t cmd_end = 0;
    while (cmd_end < *input_len && input[cmd_end] != ' ' && input[cmd_end] != '\t') {
        cmd_end++;
    }
    char prefix[MAX_WORD_LEN];
    strncpy(prefix, input, cmd_end);
    prefix[cmd_end] = '\0';

    if (strcmp(prefix, last_prefix) == 0) {
        tab_press_count++;
    } else {
        tab_press_count = 1;
        strcpy(last_prefix, prefix);
    }

    int count = 0;
    char **completions = getCompletions(trie_root, prefix, &count);

    if (count == 0) {
        write(STDOUT_FILENO, "\a", 1);
        tab_press_count = 0;
        if (completions)
            freeCompletions(completions, count);
        return;
    }

    char lcp[MAX_WORD_LEN];
    compute_lcp(completions, count, lcp);
    size_t current_prefix_len = strlen(prefix);
    size_t lcp_len = strlen(lcp);

    if (lcp_len > current_prefix_len) {
        strcpy(input, lcp);
        *input_len = lcp_len;
        if (count == 1 && *input_len < MAX_WORD_LEN - 1) {
            input[*input_len] = ' ';
            (*input_len)++;
            input[*input_len] = '\0';
        }
        tab_press_count = 0;
    } else {
        if (tab_press_count == 1) {
            write(STDOUT_FILENO, "\a", 1);
        } else if (tab_press_count >= 2) {
            write(STDOUT_FILENO, "\n", 1);
            for (int i = 0; i < count; i++) {
                write(STDOUT_FILENO, completions[i], strlen(completions[i]));
                if (i < count - 1) {
                    write(STDOUT_FILENO, "  ", 2);
                }
            }
            write(STDOUT_FILENO, "\n", 1);
            tab_press_count = 0;
        }
    }
    freeCompletions(completions, count);
}

void redraw_line(const char *prompt, const char *input) {
    printf("\r\033[2K%s%s", prompt, input);
    fflush(stdout);
}
