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
#define WHITE 0xFFFF
int main(int argc, char* argv[]) {
  gp2xInit();
  setbuf(stdout, NULL);
  printf("2D Accelerator example\n");

  uint16_t* fb = malloc(320*240*2);

  rgbSetPixelFormat(RGB565);
  rgbRegionNoBlend(REGION1);
  rgbSetRegionPosition(REGION1, 0, 0, 320, 240);
  for(int i = 320*240 ; i-- ; ) {
    *(fb+i) = WHITE;
  }

  rgbSetFbAddress((void*)fb);
  rgbToggleRegion(REGION1, true);

  // blit a picture of the sun to (0,0) with no colour key transparency
  rgbBlit(&((Graphic){&sun_bin, 60, 60, RGB565}),
	  &((Rect){0, 0, 60, 60}),
	  &((Graphic){fb, 320, 240, RGB565}),
	  0, 0, false);
  rgb2dRun();
  rgb2dWaitComplete();

  // blit a picture of the sun to (60,0) with the default colour key transparency (magenta)
  rgbBlit(&((Graphic){&sun_bin, 60, 60, RGB565}),
	  &((Rect){0, 0, 60, 60}),
	  &((Graphic){fb, 320, 240, RGB565}),
	  60, 0, true);
  rgb2dRun();
  rgb2dWaitComplete();

  // blit a picture of the sun to (120,0) with the centre colour transparent
  rgbSetTransparencyColour(0xFEC0);
  rgbBlit(&((Graphic){&sun_bin, 60, 60, RGB565}),
	  &((Rect){0, 0, 60, 60}),
	  &((Graphic){fb, 320, 240, RGB565}),
	  120, 0, true);
  rgb2dRun();
  rgb2dWaitComplete();
  
  while(1) {    
    if(btnStateDebounced() & R) {
      return 0;
    }
  }
}
