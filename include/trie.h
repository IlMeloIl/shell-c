#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_WORD_LEN 100
#define MAX_COMPLETIONS 100

typedef struct TrieNode {
    struct TrieNode *children[256];
    bool is_end;
} TrieNode;

TrieNode *createNode();
void insert(TrieNode *root, const char *word);
void collectCompletions(TrieNode *node, char *buffer, size_t len, char **completions, int *count);
char **getCompletions(TrieNode *root, const char *prefix, int *count);
void freeCompletions(char **completions, int count);
void freeTrie(TrieNode *node);
bool is_command_in_trie(TrieNode *root, const char *command);

#endif /* TRIE_H */
