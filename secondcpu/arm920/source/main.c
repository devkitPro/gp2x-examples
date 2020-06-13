#include <orcus.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  gp2xInit();
  uartPrintf("******************\n");
  uartPrintf("Second CPU example\n");
  uartPrintf("******************\n\n");

  uartPrintf("Init ARM940\n");
  arm940Init(ARM940_MEMORY_BANK);
  arm920Data[2] = 1;

  uartPrintf("Is ARM940 running? %s\n", arm940Running() ? "Yes" : "No");

  arm940Run();

  uartPrintf("Is ARM940 running? %s\n", arm940Running() ? "Yes" : "No");

  uartPrintf("Waiting for ARM940 to allocate framebuffer\n");
  while(arm920Data[2]); // wait for sync flag from 940
  uint16_t* fb = arm9x0ImportPointer(arm920Data[0] << 16 | arm920Data[1]);
  uartPrintf("ARM940 has allocated framebuffer at 0x%x\n", fb);

  rgbSetPixelFormat(RGB565);
  rgbRegionNoBlend(REGION1);
  rgbSetRegionPosition(REGION1, 0, 0, 320, 240);
  for(int i = 320*240 ; i-- ; ) {
    *(fb+i) = 0x7861;
  }
  rgbSetFbAddress((void*)fb);
  rgbToggleRegion(REGION1, true);

  while(1) {
    if(btnState()&A) {
      uartPrintf("pressed a, 920 is handling this\n");
      for(int i = 320*240 ; i-- ; ) {
	*(fb+i) = 0x7861;
      }
  }
}
}
