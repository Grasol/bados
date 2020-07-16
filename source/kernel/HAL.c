#include "HAL.h"

void HAL_portOutByte(unsigned char v, unsigned short port) {
  __asm( "out dx, al" : : "dx" (port), "al" (v) );
}

void HAL_portOutWord(unsigned short v, unsigned short port) {
  __asm( "out dx, ax": : "dx" (port), "ax" (v) );
}

void HAL_portOutDWord(unsigned int v, unsigned short port) {
  __asm( "out dx, eax": : "dx" (port), "eax" (v) );
}



unsigned char HAL_portInByte(unsigned short port) {
  unsigned char v = 0;
  __asm( "in al, dx": : "al" (v), "dx" (port) );
  return v;
}

unsigned short HAL_portInWord(unsigned short port) {
  unsigned short v = 0;
  __asm( "in ax, dx" : : "ax" (v), "dx" (port) );
  return v;
}

unsigned int HAL_portInDWord(unsigned short port) {
  unsigned char v = 0;
  __asm( "in eax, dx" : : "eax" (v), "dx" (port) );
  return v;
}