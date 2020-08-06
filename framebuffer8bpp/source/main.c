#include <stdlib.h>
#include <gp2xregs.h>
#include <orcus.h>
#include <unistd.h>

#define RED 0x0
#define BLUE 0x1

int main(int argc, char* argv[]) {
  gp2xInit();

  uint8_t* fb0 = malloc(320*240);
  uint8_t* fb1 = malloc(320*240);

  uint32_t palette[] = {0xFF000000, 0x0000FF00};
  rgbSetPalette(palette, 2, 0);
  rgbSetPixelFormat(P8BPP);
  rgbRegionNoBlend(REGION1);
  rgbSetRegionPosition(REGION1, 0, 0, 320, 240);
  for(int i = 320*240 ; i-- ; ) {
    *(fb0+i) = RED;
    *(fb1+i) = BLUE;
  }
  rgbSetFbAddress((void*)fb0);
  rgbToggleRegion(REGION1, true);

  uint32_t previousButtonState = 0;
  bool isRed = true;
  while(1) {

    // debounce buttons
    uint32_t currentButtonState = btnStateDebounced();
    uint32_t newPresses = currentButtonState&(~previousButtonState);
    previousButtonState = currentButtonState;

    if(newPresses & R) {
      return 0;
    }
    
    if(newPresses) {
      lcdWaitNextVSync();
      rgbSetFbAddress((void*)isRed ? fb1 : fb0);
      isRed = !isRed;
    }
  }
}
