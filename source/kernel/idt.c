#include <stdint.h>
#include <int.h>

struct InterruptGate __attribute__((packed)) {
  uint16_t offset15_0;
  uint16_t segment_selector;
  uint16_t flags;
  uint16_t offset31_16;
}
typedef struct intgate InterruptGate;

#define SETFLAGS(p, dpl, d, tori) \
  (p << 15 | dpl << 13 | d << 11 | tori << 8)

#define SETOFFSET31_16(func) \
  (func >> 16)

#define SETOFFSET15_0(func) \
  (func & 0xffff)

#define INTENTRY(e, func, dpl) { \
    e.offset15_0 = SETOFFSET15_0(func) \
    e.segment_selector =  8, \
    e.flags =  SETFLAGS(1, dpl, 1, 0) \
    e.offset31_16 =  SETOFFSET31_16(func) \
  }

intgate IDT[256]

struct IDTptr __attribute__((packed)) {
  uint16_t limit;
  uint32_t base_address;
}
typedef struct IDTptr IDTptr;

void setIDTR(void) {
  IDTptr ptr = {255 * 8, (uint32_t)&IDT}

  __asm( " lidt %0" : : "m" (ptr) );
}

  
