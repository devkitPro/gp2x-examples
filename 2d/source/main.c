#include <stdio.h>
#include <stdlib.h>
#include <gp2xregs.h>
#include <orcus.h>
#include <unistd.h>
#include <2d.h>
#include <sun_bin.h>

#define RED 0xF800
#define GREEN 0x07E0
#define BLACK 0x0000

int main(int argc, char* argv[]) {
  gp2xInit();
  setbuf(stdout, NULL);
  printf("2D Accelerator example\n");

  uint16_t* fb0 = malloc(320*240*2);
  uint16_t* fb1 = malloc(320*240*2);
  printf("FB0: %p, FB1 %p, %p\n", fb0, fb1, &FREG32(DSTCTRL));

  rgbSetPixelFormat(RGB565);
  rgbRegionNoBlend(REGION1);
  rgbSetRegionPosition(REGION1, 0, 0, 320, 240);
  for(int i = 320*240 ; i-- ; ) {
    *(fb0+i) = BLACK;
    *(fb1+i) = BLACK;
  }

  uint16_t* fb = fb0;
  uint16_t* nextFb = fb1;
  rgbSetFbAddress((void*)fb);
  rgbToggleRegion(REGION1, true);

  uint32_t previousButtonState = 0;
  while(1) {    
    uint32_t currentButtonState = btnStateDebounced();
    uint32_t newPresses = currentButtonState&(~previousButtonState);
    previousButtonState = currentButtonState;

    bool redraw = false;

    if(newPresses & A) {
      printf("Blitting sun\n");
      rgbBlit(&((Graphic){&sun_bin, 60, 60, RGB565}),
	      &((Rect){0, 0, 60, 60}),
	      &((Graphic){nextFb, 320, 240, RGB565}),
	      0, 0, false);
      rgb2dRun();
      rgb2dWaitComplete();
      redraw = true;
    }
    
    if(redraw) {
      lcdWaitNextVSync();
      rgbSetFbAddress((void*)nextFb);
      uint16_t* curFb = fb;
      fb = nextFb;
      nextFb = curFb;
    }
  }
}
