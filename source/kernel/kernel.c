#include "terminal.h"

// main function of bados kernel
void _start() {
  char str[] = "Hello, World!!!";

  TERM_clearScreen();
  TERM_updateCursorPos(1, 1);
  TERM_putText(str);

  for(;;);
} 

