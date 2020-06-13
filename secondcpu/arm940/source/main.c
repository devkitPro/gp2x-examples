// this doesn't do much, it simply proves the 940 is running
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <orcus.h>

extern volatile void* __is_arm940;
int main() {
  void* fb = (void*)0x10000;// malloc(320*240*2); // TODO - investigate why malloc sometimes doesn't work
  
  arm920Data[0] = (((uint32_t)fb) >> 16);
  arm920Data[1] = (((uint32_t)fb)&0xFFFF);
  usleep(1000000);
  arm920Data[2] = 0; // clear flag to indicate we've done our work

  uint32_t btn = btnState();
  while(1) {
    btn = btnState();
    if(btn & B) {
      uartPrintf("pressed b, 940 is handling this\n");
      for(int i = 320*240 ; i-- ; ) {
	*(((uint16_t*)fb)+i) = 0xD6A4;
      }
    }
  }
}
