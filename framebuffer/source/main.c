#include <stdlib.h>
#include <gp2xregs.h>
#include <orcus.h>
#include <unistd.h>

#define RED 0xF800
#define GREEN 0x07E0

int main(int argc, char* argv[]) {
  gp2xInit();

  uint16_t* fb0 = malloc(320*240*2);
  uint16_t* fb1 = malloc(320*240*2);

  rgbSetPixelFormat(RGB565);
  rgbRegionNoBlend(REGION1);
  rgbSetRegionPosition(REGION1, 0, 0, 320, 240);
  for(int i = 320*240 ; i-- ; ) {
    *(fb0+i) = RED;
    *(fb1+i) = GREEN;
  }
  rgbSetFbAddress((void*)fb0);
  rgbToggleRegion(REGION1, true);

  uint32_t previousButtonState = 0;
  bool isRed = true;
  while(1) {

    // debounce buttons
    uint32_t currentButtonState = btnState();
    uint32_t nextButtonState;
    while(1) {
      usleep(5000);
      nextButtonState = btnState();
      if(nextButtonState == currentButtonState) {
	break;
      }
      currentButtonState = nextButtonState;
    }

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
