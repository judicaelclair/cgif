#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "cgif.h"

#define WIDTH  0xFFFFUL
#define HEIGHT 0xFFFFUL

/* This is an example code that creates a GIF-image with all green pixels. */
int main(void) {
  CGIF*         pGIF;
  CGIF_Config     gConfig;
  CGIF_FrameConfig   fConfig;
  uint8_t*      pImageData;
  uint8_t       aPalette[] = { 0xFF, 0x00, 0x00 };
  cgif_result   r;
  uint16_t      numColors  = 1; // number of colors in aPalette  
  //
  // create an image
  pImageData = malloc(WIDTH * HEIGHT);   // actual image data
  memset(pImageData, 0, WIDTH * HEIGHT);
  //
  // create new GIF
  memset(&gConfig, 0, sizeof(CGIF_Config));
  gConfig.width                   = WIDTH;
  gConfig.height                  = HEIGHT;
  gConfig.pGlobalPalette          = aPalette;
  gConfig.numGlobalPaletteEntries = numColors;
  gConfig.path                    = "max_size.gif";
  pGIF = cgif_newgif(&gConfig);
  if(pGIF == NULL) {
    fputs("failed to create new GIF via cgif_newgif()\n", stderr);
    return 1;
  }
  //
  // add frames to GIF
  memset(&fConfig, 0, sizeof(CGIF_FrameConfig));
  fConfig.pImageData = pImageData;
  r = cgif_addframe(pGIF, &fConfig);
  free(pImageData);  
  //
  // free allocated space at the end of the session
  r = cgif_close(pGIF);

  // check for errors
  if(r != CGIF_OK) {
    fprintf(stderr, "failed to create GIF. error code: %d\n", r);
    return 2;
  }
  return 0;
}
