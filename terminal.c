#include "terminal.h"
#include <unistd.h>
#include <termios.h>

struct termios orig_termios;

void restore_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}
