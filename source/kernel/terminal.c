#include "terminal.h"
#include "HAL.h"

// Function clears screen. It doesn't displace cursor 
void TERM_clearScreen(void) {
  char text_color = 0x0f;
  char *text_vram = (void*)TERM_VRAM;
  for(int i = 0; i < 80 * 25; i++) {
    text_vram[i * 2 + 0] = ' ';
    text_vram[i * 2 + 1] = (char)text_color;
  }
}

void TERM_putText(char *text) {
  unsigned short curs_pos = TERM_getCursorPos();
  for(; *text != 0; text++) {
    TERM_putChar(*text, curs_pos);
    curs_pos++;
  }
  TERM_updateCursorPos(curs_pos, 0);
}

void TERM_putChar(char c, unsigned short curs_pos) {
  char *cursor = (char*)(curs_pos * 2 + TERM_VRAM);
  cursor[0] = c;
  __asm( "xchg bx, bx" );
  cursor[1] = (char)text_color; // Fix, because don't work
}

void TERM_updateCursorPos(int x, int y) {
  unsigned short offset = y * 80 + x;

  HAL_portOutByte(15, VGA_PORT_3D4_REGISTER_INDEX); // set
  HAL_portOutByte(offset & 0xff, VGA_PORT_3D5_REGISTER_DATA);

  HAL_portOutByte(14, VGA_PORT_3D4_REGISTER_INDEX);
  HAL_portOutByte((unsigned char)(offset >> 8), VGA_PORT_3D5_REGISTER_DATA);  
}

unsigned short TERM_getCursorPos(void) {
  unsigned short pos = 0;
  HAL_portOutByte(15, VGA_PORT_3D4_REGISTER_INDEX);
  pos |= HAL_portInByte(VGA_PORT_3D5_REGISTER_DATA);

  HAL_portOutByte(14, VGA_PORT_3D4_REGISTER_INDEX);
  pos |= ((unsigned short)HAL_portInByte(VGA_PORT_3D5_REGISTER_DATA)) << 8;
  return pos;
}

