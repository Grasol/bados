#pragma once

void HAL_portOutByte(unsigned char v, unsigned short port);
void HAL_portOutWord(unsigned short v, unsigned short port);
void HAL_portOutDWord(unsigned int v, unsigned short port);
unsigned char HAL_portInByte(unsigned short port);
unsigned short HAL_portInWord(unsigned short port);
unsigned int HAL_portInDWord(unsigned short port);