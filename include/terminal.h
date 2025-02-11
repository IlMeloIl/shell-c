#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>

extern struct termios orig_termios;
void restore_terminal(void);

#endif /* TERMINAL_H */
