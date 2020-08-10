// this doesn't do much, it simply proves the 940 is running
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <orcus.h>

extern volatile void* __is_arm940;
int main() {
  gp2xInit();
  puCachesInitOn();
  uint16_t* fb = (uint16_t*) malloc(320*240*2);
  
  arm920Data[0] = (((uint32_t)fb) >> 16);
  arm920Data[1] = (((uint32_t)fb)&0xFFFF);
  uartPrintf("FB allocated at 0x%x, globally this is the same as 0x%x\n",fb, exportPointer(fb));
  usleep(5000000);

  rgbSetPixelFormat(RGB565);
  rgbRegionNoBlend(REGION1);
  rgbSetRegionPosition(REGION1, 0, 0, 320, 240);
  for(int i = 320*240 ; i-- ; ) {
    *(fb+i) = 0x7861;
  }
  rgbSetFbAddress((void*)exportPointer(fb));
  rgbToggleRegion(REGION1, true);
  
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
