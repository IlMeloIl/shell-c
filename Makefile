CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L

OBJS = main.o trie.o builtin.o util.o input.o redirection.o terminal.o command.o

all: shell

shell: $(OBJS)
	$(CC) $(CFLAGS) -o shell $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) shell
