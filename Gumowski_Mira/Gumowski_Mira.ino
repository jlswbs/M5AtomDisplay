// Gumowski-Mira chaotic map //

#include <M5AtomDisplay.h>

#define WIDTH   1280
#define HEIGHT  720

M5AtomDisplay display (WIDTH, HEIGHT);

float randomf(float minf, float maxf) {return minf + (esp_random()%(1UL << 31))*(maxf - minf) / (1UL << 31);}  

#define ITER  2000

  float a = 0.000001f;
  float b = 0.05f;
  float m = -0.031f;
  float x = 0.0f;
  float y = 0.5f;
  uint16_t coll = TFT_WHITE;

float mira(float x) {
  
  float xx = x * x;
  return (m * x) + ((2.0f * (1.0f - m) * xx) / (1.0f + xx));
  
}

void setup(void){

  srand(time(NULL));
  
  display.init();
  display.startWrite();
  display.setColorDepth(16);
  display.fillScreen(TFT_BLACK);

  m = randomf(-0.95f, -0.05f);

}

void loop(void){

  display.waitDisplay();

  for (int i = 0; i < ITER; i++){

    float nx = x;
    float ny = y;
    
    x = ny + a * (1.0f - (b * powf(ny,2.0f))) * ny + mira(x);
    y = -nx + mira(x);

    uint16_t xx = (WIDTH/2) + (10.0f * x); 
    uint16_t yy = (HEIGHT/2) + (10.0f * y);
    
    if (coll == TFT_WHITE && xx == (WIDTH/2)+4){
      display.clear();
      coll = TFT_WHITE;
      m = randomf(-0.95f, -0.05f);
    } else if (xx == yy) coll = esp_random()%65535;
      
    if(xx > 0 && xx < WIDTH && yy > 0 && yy < HEIGHT) display.writeFillRect(xx, yy, 1, 1, coll);
    
  }
  
  display.display();

}
