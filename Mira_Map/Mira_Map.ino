// Mira chaotic map //

#include <M5AtomDisplay.h>

#define WIDTH   1280
#define HEIGHT  720

M5AtomDisplay display (WIDTH, HEIGHT);

float randomf(float minf, float maxf) {return minf + (esp_random()%(1UL << 31))*(maxf - minf) / (1UL << 31);}  

#define ITER  2000

  float x = 1.0f;
  float y = 0.0f;
  float f = 0.0f;
  float a = 0.2f;
  uint16_t coll = TFT_WHITE;

void setup(void){

  srand(time(NULL));
  
  display.init();
  display.startWrite();
  display.setColorDepth(16);
  display.fillScreen(TFT_BLACK);

  a = randomf(0.099f, 0.499f);

}

void loop(void){

  display.waitDisplay();

  for (int i = 0; i < ITER; i++){

    float nx = x;
    float ny = y;
        
    x = ny + f;
    f = a * x + ((2.0f*(1.0f-a)*powf(x,2.0f))/(1.0f+powf(x,2.0f)));
    y = -nx + f;

    uint16_t xx = (WIDTH/2) + (24.0f * x); 
    uint16_t yy = (HEIGHT/2) + (24.0f * y);
    
    if (coll == TFT_WHITE && xx == (WIDTH/2)+8){
      display.clear();
      coll = TFT_WHITE;
      a = randomf(0.099f, 0.499f);
    } else if (xx == yy) coll = esp_random()%65535;
      
    if(xx > 0 && xx < WIDTH && yy > 0 && yy < HEIGHT) display.writeFillRect(xx, yy, 1, 1, coll);
    
  }
  
  display.display();

}
