#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TrieNode *createNode() {
    TrieNode *node = malloc(sizeof(TrieNode));
    if (node == NULL) {
        perror("malloc failed");
        exit(1);
    }
    node->is_end = false;
    for (int i = 0; i < 256; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void insert(TrieNode *root, const char *word) {
    TrieNode *current = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = (unsigned char)word[i];
        if (current->children[index] == NULL) {
            current->children[index] = createNode();
        }
        current = current->children[index];
    }
    current->is_end = true;
}

void collectCompletions(TrieNode *node, char *buffer, size_t len, char **completions, int *count) {
    if (node == NULL)
        return;
    if (node->is_end) {
        completions[*count] = strdup(buffer);
        if (completions[*count] == NULL) {
            perror("strdup failed");
            exit(1);
        }
        (*count)++;
        if (*count >= MAX_COMPLETIONS)
            return;
    }
    for (int i = 0; i < 256; i++) {
        if (node->children[i] != NULL) {
            if (len + 1 >= MAX_WORD_LEN)
                continue;
            buffer[len] = (char)i;
            buffer[len + 1] = '\0';
            collectCompletions(node->children[i], buffer, len + 1, completions, count);
            buffer[len] = '\0';
            if (*count >= MAX_COMPLETIONS)
                return;
        }
    }
}

char **getCompletions(TrieNode *root, const char *prefix, int *count) {
    *count = 0;
    TrieNode *current = root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = (unsigned char)prefix[i];
        if (current->children[index] == NULL)
            return NULL;
        current = current->children[index];
    }
    char **completions = malloc(MAX_COMPLETIONS * sizeof(char *));
    if (completions == NULL) {
        perror("malloc failed");
        exit(1);
    }
    char buffer[MAX_WORD_LEN];
    strcpy(buffer, prefix);
    collectCompletions(current, buffer, strlen(prefix), completions, count);
    return completions;
}

void freeCompletions(char **completions, int count) {
    for (int i = 0; i < count; i++) {
        free(completions[i]);
    }
    free(completions);
}

void freeTrie(TrieNode *node) {
    if (node == NULL)
        return;
    for (int i = 0; i < 256; i++) {
        if (node->children[i])
            freeTrie(node->children[i]);
    }
    free(node);
}

bool is_command_in_trie(TrieNode *root, const char *command) {
    TrieNode *current = root;
    for (int i = 0; command[i] != '\0'; i++) {
        int index = (unsigned char)command[i];
        if (current->children[index] == NULL) {
            return false;
        }
        current = current->children[index];
    }
    return current->is_end;
}
