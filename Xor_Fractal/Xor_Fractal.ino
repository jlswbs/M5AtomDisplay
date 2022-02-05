// Xor fractal //

#include <M5AtomDisplay.h>

#define WIDTH   1280
#define HEIGHT  720

M5AtomDisplay display (WIDTH, HEIGHT);

  uint16_t coll;
  long t;


void setup(void){

  display.init();
  display.startWrite();
  display.setColorDepth(16);
  display.fillScreen(TFT_BLACK);

}

void loop(void){

  display.waitDisplay();
  
  for (int y = 0; y < HEIGHT/16; y++) {
    for (int x = 0; x < WIDTH/16; x++) {
      
      coll = (x ^ y) * t;

      display.writeFillRect(16*x, 16*y, coll%16, coll%16, coll);
            
    }
  }
  
  display.display();

  t++;

}
