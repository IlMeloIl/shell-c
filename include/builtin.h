#ifndef BUILTIN_H
#define BUILTIN_H

typedef int (*builtin_handler)(char **tokens, int token_count);

typedef struct {
    const char *name;
    builtin_handler handler;
} Builtin;

int builtin_exit(char **tokens, int token_count);
int builtin_echo(char **tokens, int token_count);
int builtin_type(char **tokens, int token_count);
int builtin_pwd(char **tokens, int token_count);
int builtin_cd(char **tokens, int token_count);

extern Builtin builtins[];
extern int num_builtins;

#endif /* BUILTIN_H */
