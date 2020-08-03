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
#define MAGENTA 0xF81F

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

  Graphic* screen = &((Graphic){fb, 320, 240, RGB565});
  Graphic* sun = &((Graphic){sun_bin, 60, 60, RGB565});

  // blit a picture of the sun to (0,0) with no colour key transparency
  rgbBlit(sun,
	  &((Rect){0, 0, 60, 60}),
	  screen,
	  0, 0, false);
  rgb2dRun();
  rgb2dWaitComplete();

  // blit a picture of the sun to (60,0) with the default colour key transparency (magenta)
  rgbBlit(sun,
	  &((Rect){0, 0, 60, 60}),
	  screen,
	  60, 0, true);
  rgb2dRun();
  rgb2dWaitComplete();

  // blit a picture of the sun to (120,0) with the centre colour transparent
  rgbSetTransparencyColour(0xFEC0);
  rgbBlit(sun,
	  &((Rect){0, 0, 60, 60}),
	  screen,
	  120, 0, true);
  rgb2dRun();
  rgb2dWaitComplete();

  // fill a 60x60 square at (180,0) with a green colour
  rgbSolidFill(screen, &((Rect){180, 0, 60, 60}), GREEN);
  rgb2dRun();
  rgb2dWaitComplete();

  // fill a 60x60 square with chequerboard pattern at (240,0)
  pattern[0] = 0xAA55AA55;
  pattern[1] = 0xAA55AA55;
  rgbPatternFill(screen, &((Rect){240, 0, 60, 60}), &((RasterPattern){GREEN, RED, B1BPP, 0}), false);
  rgb2dRun();
  rgb2dWaitComplete();

  // draw sun at (0,60) excluding set bits in pattern resulting in a shaded effect
  pattern[0] = 0xAA55AA55;
  pattern[1] = 0xAA55AA55;
  rgbSetTransparencyColour(MAGENTA);
  rgbRasterOp(sun, &((Rect){0, 0, 60, 60}),
	      screen, &((Rect){0, 60, 60, 60}),
	      ROP_EXCLUDEPAT,
	      &((RasterPattern){0xFFFF, BLACK, B1BPP, 0}),
	      true, 0, 0);
  rgb2dRun();
  rgb2dWaitComplete();
  
  while(1) {    
    if(btnStateDebounced() & R) {
      return 0;
    }
  }
}
