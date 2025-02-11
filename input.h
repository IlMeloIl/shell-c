#ifndef INPUT_H
#define INPUT_H

#include "trie.h"
#include <stddef.h>

void redraw_line(const char *prompt, const char *input);
void handle_tab(char *input, size_t *input_len, TrieNode *trie_root);

#endif /* INPUT_H */
