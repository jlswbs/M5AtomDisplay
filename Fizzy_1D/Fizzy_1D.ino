// Fizzy 1D cellular automata //

#include <M5AtomDisplay.h>

#define WIDTH   1280
#define HEIGHT  720

M5AtomDisplay display (WIDTH, HEIGHT);

uint16_t RGB565(byte R, byte G, byte B){ return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );}

  uint16_t coll;
  int Calm = 233;
  float a = 4.7f;
  int CellIndex = 0;
  float CellVal[WIDTH];


void setup(void){

  srand(time(NULL));
  
  display.init();
  display.startWrite();
  display.setColorDepth(16);
  display.fillScreen(TFT_BLACK);

}


void loop(void){

  Calm = 16 + esp_random()%233;
  for (int i = 0; i < WIDTH; i++) CellVal[i] = esp_random()%128;

  display.waitDisplay();

  for (int y = 0; y < HEIGHT; y++) {
    
    for (int x = 0; x < WIDTH; x++) {

      CellIndex = (CellIndex+1)%WIDTH;

      uint16_t coll = a * CellVal[CellIndex];
      display.writeFillRect(x, y, 1, 1, RGB565(coll, coll, coll));

      int below      = (CellIndex+1)%WIDTH;
      int above      = (CellIndex+WIDTH-1)%WIDTH;
      int left       = (CellIndex+WIDTH-1)%WIDTH;
      int right      = (CellIndex+1)%WIDTH;
      int aboveright = ((CellIndex-1) + 1 + WIDTH)%WIDTH;
      int aboveleft  = ((CellIndex-1) - 1 + WIDTH)%WIDTH;
      int belowright = ((CellIndex+1) + 1 + WIDTH)%WIDTH;
      int belowleft  = ((CellIndex+1) - 1 + WIDTH)%WIDTH;

      float NeighbourMix = powf((CellVal[left]*CellVal[right]*CellVal[above]*CellVal[below]*CellVal[belowleft]*CellVal[belowright]*CellVal[aboveleft]*CellVal[aboveright]),0.125f);
      CellVal[CellIndex] = fmod(sqrtf(CellVal[CellIndex]*NeighbourMix)+0.5f, Calm);

    }  
  }

  display.display();

}
