#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "cgif.h"

#define WIDTH  100
#define HEIGHT 100

/* This is an example code that creates a GIF-animation with a moving stripe-pattern. */
int main(void) {
  CGIF*          pGIF;
  CGIF_Config      gConfig;
  CGIF_FrameConfig    fConfig;
  uint8_t*      pImageData;
  uint8_t       aPalette[] = {
    0x00, 0x00, 0x00, // black
    0xFF, 0xFF, 0xFF, // white
    0xFF, 0x00, 0x00, // red
    0x00, 0xFF, 0x00, // green
    0x00, 0x00, 0xFF, // blue
  };
  cgif_result r;
  uint8_t numColors   = 5;  // number of colors in aPalette
  int numFrames       = 30; // number of frames in the video
  
  memset(&gConfig, 0, sizeof(CGIF_Config));
  memset(&fConfig, 0, sizeof(CGIF_FrameConfig));
  gConfig.attrFlags               = CGIF_ATTR_IS_ANIMATED; // set needed attribution flag (as GIF is animated) 
  gConfig.width                   = WIDTH;
  gConfig.height                  = HEIGHT;
  gConfig.pGlobalPalette          = aPalette;
  gConfig.numGlobalPaletteEntries = numColors;
  gConfig.path                    = "animated_stripe_pattern.gif";
  //
  // create new GIF
  pGIF = cgif_newgif(&gConfig);
  if(pGIF == NULL) {
    fputs("failed to create new GIF via cgif_newgif()\n", stderr);
    return 1;
  }
  //
  // add frames to GIF
  pImageData = malloc(WIDTH * HEIGHT);         // actual image data
  fConfig.genFlags   = CGIF_FRAME_GEN_USE_DIFF_WINDOW | CGIF_FRAME_GEN_USE_TRANSPARENCY;
  fConfig.pImageData = pImageData;             // set pointer to image data
  fConfig.delay      = 10;                     // set time before next frame (in units of 0.01 s)
  for (int f = 0; f < numFrames; ++f) {
    for (int i = 0; i < (WIDTH * HEIGHT); ++i) {
    	pImageData[i] = (unsigned char)((f + i % WIDTH) % numColors); // ceate a moving stripe pattern
    }
    r = cgif_addframe(pGIF, &fConfig); // append the new frame
    if(r != CGIF_OK) {
      break;
    }
  }
  free(pImageData);
  //
  // write GIF to file
  r = cgif_close(pGIF);                  // free allocated space at the end of the session

  // check for errors
  if(r != CGIF_OK) {
    fprintf(stderr, "failed to create GIF. error code: %d\n", r);
    return 2;
  }
  return 0;
}
