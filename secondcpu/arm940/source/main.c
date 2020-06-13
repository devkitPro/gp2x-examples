// this doesn't do much, it simply proves the 940 is running
#include <stdint.h>
#include <orcus.h>

volatile uint32_t* dest = (uint32_t*) 0x0;

int main() {
    *dest = 0x87654321;
  while(1) {
    *dest = *dest - 1;
    uartPrintf("hello from 940\n");
  }
}
