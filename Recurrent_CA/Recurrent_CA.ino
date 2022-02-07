// Recurrent cellular automata //

#include <M5AtomDisplay.h>

#define RND   12
#define VAL   15
#define COL   5
#define WIDTH   1280
#define HEIGHT  720

M5AtomDisplay display (WIDTH, HEIGHT);

  uint16_t coll;
  uint8_t parent[WIDTH]; 
  uint8_t child[WIDTH];
  int count;
  int a[VAL];


void setup(void){

  srand(time(NULL));
  
  display.init();
  display.startWrite();
  display.setColorDepth(16);
  display.fillScreen(TFT_BLACK);

}

void loop(void){

  for(int i = 0; i < VAL; i++) a[i] = 1 + esp_random()%RND;
  for(int i = 0; i < WIDTH; i++) parent[i] = esp_random()%COL;

  display.waitDisplay();
  
  for (int y = 0; y < HEIGHT; y++) {
 
    for (int x = 0; x < WIDTH; x++) {
          
      if (x == 0) count = parent[WIDTH-1] + parent[0] + parent[1];
      else if (x == WIDTH-1) count = parent[WIDTH-2] + parent[WIDTH-1] + parent[0];
      else count = parent[x-1] + parent[x] + parent[x+1];        
            
      if(count == a[0] || count == a[1] || count == a[2]) child[x] = 0;
      if(count == a[3] || count == a[4] || count == a[5]) child[x] = 1;
      if(count == a[6] || count == a[7] || count == a[8]) child[x] = 2;
      if(count == a[9] || count == a[10] || count == a[11]) child[x] = 3;
      if(count == a[12] || count == a[13] || count == a[14]) child[x] = 4;
               
      if (child[x] == 0) coll = TFT_RED;
      if (child[x] == 1) coll = TFT_WHITE;
      if (child[x] == 2) coll = TFT_GREEN;
      if (child[x] == 3) coll = TFT_BLACK;
      if (child[x] == 4) coll = TFT_BLUE;
        
      display.writeFillRect(x, y, 1, 1, coll);
                                                   
    }

    for(int i = 0; i < WIDTH; i++) parent[i] = child[(i+1)%WIDTH];                              

  }

  display.display();

}
