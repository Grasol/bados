#pragma once

struct TextModeScreenContext {  
  int TERM_VRAM;
  char text_color;
  short cursor_pos_x;
  short cursor_pos_y;
} TextScreen;

enum {
  VGA_PORT_3D4_REGISTER_INDEX = 0x3d4,
  VGA_PORT_3D5_REGISTER_DATA = 0x3d5
};

void TERM_init(void);
void TERM_clearScreen(void);
void TERM_putText(char *text);
void TERM_putChar(char c, unsigned short curs_pos);
void TERM_updateCursorPos(int x, int y);
unsigned short TERM_getCursorPos(void);
