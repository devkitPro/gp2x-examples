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
  rgbRasterRun();
  rgbRasterWaitComplete();

  // blit a picture of the sun to (60,0) with the default colour key transparency (magenta)
  rgbBlit(sun,
	  &((Rect){0, 0, 60, 60}),
	  screen,
	  60, 0, true);
  rgbRasterRun();
  rgbRasterWaitComplete();

  // blit a picture of the sun to (120,0) with the centre colour transparent
  rgbSetTransparencyColour(0xFEC0);
  rgbBlit(sun,
	  &((Rect){0, 0, 60, 60}),
	  screen,
	  120, 0, true);
  rgbRasterRun();
  rgbRasterWaitComplete();

  // fill a 60x60 square at (180,0) with a green colour
  rgbSolidFill(screen, &((Rect){180, 0, 60, 60}), GREEN);
  rgbRasterRun();
  rgbRasterWaitComplete();

  // fill a 60x60 square with chequerboard pattern at (240,0)
  pattern[0] = 0xAA55AA55;
  pattern[1] = 0xAA55AA55;
  rgbPatternFill(screen, &((Rect){240, 0, 60, 60}), &((RasterPattern){GREEN, RED, B1BPP, 0}), false);
  rgbRasterRun();
  rgbRasterWaitComplete();

  // draw sun at (0,60) excluding set bits in pattern with unset bits being black resulting in a shaded effect
  pattern[0] = 0xAA55AA55;
  pattern[1] = 0xAA55AA55;
  rgbSetTransparencyColour(MAGENTA);
  rgbRasterOp(sun, &((Rect){0, 0, 60, 60}),
	      screen, &((Rect){0, 60, 60, 60}),
	      ROP_DPSao,
	      &((RasterPattern){0xFFFF, BLACK, B1BPP, 0}),
	      true, 0, 0);
  rgbRasterRun();
  rgbRasterWaitComplete();

  // draw sun at (0,120) excluding set bits in pattern with unset bits being black resulting in a lightning effect
  rgbRasterOp(sun, &((Rect){0, 0, 60, 60}),
	      screen, &((Rect){0, 120, 60, 60}),
	      ROP_SPno,
	      &((RasterPattern){0xFFFF, BLACK, B1BPP, 0}),
	      false, 0, 0);
  rgbRasterRun();
  rgbRasterWaitComplete();
  
  // draw the sun at (60,60) rotated 90 degrees
  rgbRotBlit(sun, &((Rect){0, 0, 60, 60}),
	     screen, 60, 60, DEG90);
  rgbRotRun();
  rgbRotWaitComplete();

  // draw the sun at (120,60) rotated 180 degrees
  rgbRotBlit(sun, &((Rect){0, 0, 60, 60}),
	     screen, 120, 60, DEG180);
  rgbRotRun();
  rgbRotWaitComplete();

  // draw the sun at (180,60) rotated 270 degrees
  rgbRotBlit(sun, &((Rect){0, 0, 60, 60}),
	     screen, 180, 60, DEG270);
  rgbRotRun();
  rgbRotWaitComplete();

  // draw the sun at (240,60) first into an intermediate buffer rotated 270 degrees, then onto the screen with transparency applied
  uint16_t intBuf[60*60];
  Graphic* intBufG = &((Graphic){&intBuf, 60, 60, RGB565});
  rgbRotBlit(sun, &((Rect){0, 0, 60, 60}),
	     intBufG, 0, 0, DEG270);
  rgbRotRun();
  rgbRotWaitComplete();

  rgbBlit(intBufG,
	  &((Rect){0, 0, 60, 60}),
	  screen,
	  240, 60, true);
  rgbRasterRun();
  rgbRasterWaitComplete();

  // draw a font letter to (8, 188)
  uint8_t letterA[8] = {0x18, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42};
  Graphic* letterAG = &((Graphic){letterA, 8, 8, B1BPP});
  rgbBlit1bpp(letterAG, &((Rect){0, 0, 8, 8}), screen, 8, 188, true, RED, MAGENTA);
  rgbRasterRun();
  rgbRasterWaitComplete();
  
  // done
  printf("Finished drawing!\n\n");
  
  while(1) {    
    if(btnStateDebounced() & R) {
      return 0;
    }
  }
}
