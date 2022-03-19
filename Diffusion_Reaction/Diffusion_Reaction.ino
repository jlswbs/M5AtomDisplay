// 1D Diffusion-Reaction //

#include <M5AtomDisplay.h>

#define WIDTH   1280
#define HEIGHT  720

M5AtomDisplay display (WIDTH, HEIGHT);

uint16_t RGB565(byte R, byte G, byte B){ return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );}
float randomf(float minf, float maxf) {return minf + (esp_random()%(1UL << 31))*(maxf - minf) / (1UL << 31);}  
  
  float A[WIDTH]; 
  float I[WIDTH];
  float D2A[WIDTH]; 
  float D2I[WIDTH];

  float p[6] = {0.5f, 2.0f, 2.0f, 2.0f, 1.0f, 0.0f};
  float a = 5.0f;
  float dt = 0.05f;


void rndrule(){

  p[0] = randomf(0.0f, 1.0f);
  p[1] = randomf(0.0f, 15.0f);
  p[2] = randomf(0.0f, 4.0f);
  p[3] = randomf(0.0f, 15.0f);
  p[4] = randomf(0.0f, 4.0f);
  p[5] = randomf(0.0f, 2.0f);

  a = randomf(1.9f, 7.9f);
   
  for(int i=0;i<WIDTH; i++) {
    
    A[i] = randomf(0.0f, 1.0f);
    I[i] = randomf(0.0f, 1.0f);
    
  }
         
}

void setup(void){

  srand(time(NULL));
  
  display.init();
  display.startWrite();
  display.setColorDepth(16);
  display.fillScreen(TFT_BLACK);

}


void loop(void){

  rndrule();

  display.waitDisplay();

  for(int j=0;j<HEIGHT; j++) {
    
    for(int i=1;i<WIDTH-1; i++) {
      
      D2A[i] = A[i-1] + A[i+1] - 2.0f * A[i];
      D2I[i] = I[i-1] + I[i+1] - 2.0f * I[i];
 
    }
  
    D2A[0] = A[1] - A[0]; 
    D2I[0] = I[1] - I[0]; 

    for(int i=0;i<WIDTH; i++) {
    
      A[i] = A[i] + dt * (a * A[i] *A[i] * A[i] / (I[i] * I[i]) + p[0] - p[1] * A[i] + p[2] * D2A[i]);
      I[i] = I[i] + dt * (A[i] * A[i] *A[i] - p[3] * I[i] + p[4] * D2I[i] + p[5]);  
        
      uint8_t coll = 250 - (50.0f * (A[i]+I[i]));
      display.writeFillRect(i, j, 1, 1, RGB565(coll, coll, coll));
      
    }

  }

  display.display();

}
