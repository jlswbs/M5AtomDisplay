// Multi-Scale Turing patterns //

#include <M5AtomDisplay.h>

  #define WFULL   1280
  #define HFULL   720
  #define WIDTH   WFULL/8
  #define HEIGHT  HFULL/8

M5AtomDisplay display (WFULL, HFULL);

uint16_t RGB565(byte R, byte G, byte B){ return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );}

  uint16_t coll;
  #define n (WIDTH * HEIGHT)

  #define min(a,b) ((a)<(b)?(a):(b))
  #define max(a,b) ((a)>(b)?(a):(b))

  uint16_t size = ((2*WIDTH) * (2*HEIGHT));  
 
  int8_t *bestLevel = NULL;
  float *grid = NULL;
  float *blurBuffer = NULL;
  float *variation = NULL;
  float *bestVariation = NULL;
  float *activator = NULL;
  float *inhibitor = NULL;
  float *swap = NULL;
  bool *direction = NULL;
  float *stepSizes = NULL;
  int8_t *radii = NULL;

  float randomf(float minf, float maxf) {return minf + (esp_random()%(1UL << 31)) * (maxf - minf) / (1UL << 31);}

  int level, i, x, y;
  int blurlevels;
 
  float base;
  int levels;
  float stepScale;
  float stepOffset;
  float blurFactor;



void setup(void){

  srand(time(NULL));
  
  display.init();
  display.startWrite();
  display.setColorDepth(16);
  display.fillScreen(TFT_BLACK);

  bestLevel = (int8_t*)ps_malloc(size);
  grid = (float*)ps_malloc(size);
  blurBuffer = (float*)ps_malloc(size);
  variation = (float*)ps_malloc(size);
  bestVariation = (float*)ps_malloc(size);
  activator = (float*)ps_malloc(size);
  inhibitor = (float*)ps_malloc(size);
  swap = (float*)ps_malloc(size);
  direction = (bool*)ps_malloc(size);
  stepSizes = (float*)ps_malloc(WIDTH);
  radii = (int8_t*)ps_malloc(WIDTH);

  rndrule();

}


void loop(void){

  display.waitDisplay();
  
  memcpy(activator, grid, 4*n);
 
  for (level = 0; level < levels - 1; level++) {
      
    int radius = radii[level];
    
    if(level <= blurlevels){       
      for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
          int t = y * WIDTH + x;
          if (y == 0 && x == 0) blurBuffer[t] = activator[t];
          else if (y == 0) blurBuffer[t] = blurBuffer[t - 1] + activator[t];
          else if (x == 0) blurBuffer[t] = blurBuffer[t - WIDTH] + activator[t];
          else blurBuffer[t] = blurBuffer[t - 1] + blurBuffer[t - WIDTH] - blurBuffer[t - WIDTH - 1] + activator[t];
        }
      }
    }

    for (y = 0; y < HEIGHT; y++) {
      for (x = 0; x < WIDTH; x++) {
        int minx = max(0, x - radius);
        int maxx = min(x + radius, WIDTH - 1);
        int miny = max(0, y - radius);
        int maxy = min(y + radius, HEIGHT - 1);
        int area = (maxx - minx) * (maxy - miny);
       
        int nw = miny * WIDTH + minx;
        int ne = miny * WIDTH + maxx;
        int sw = maxy * WIDTH + minx;
        int se = maxy * WIDTH + maxx;
       
        int t = y * WIDTH + x;
        inhibitor[t] = (blurBuffer[se] - blurBuffer[sw] - blurBuffer[ne] + blurBuffer[nw]) / area;
      }
    }
    
    for (i = 0; i < n; i++) {
      float variation = fabs(activator[i] - inhibitor[i]);
      if (level == 0 || variation < bestVariation[i]) {
        bestVariation[i] = variation;
        bestLevel[i] = level;
        direction[i] = activator[i] > inhibitor[i];
      }
    }
 
   if(level==0) {
      memcpy(activator, inhibitor, 4*n);
    } else {
      memcpy(swap, activator, 4*n);
      memcpy(activator, inhibitor, 4*n);
      memcpy(inhibitor, swap, 4*n);
    }
            
  } 

  float smallest = MAXFLOAT;
  float largest = -MAXFLOAT;

  for (i = 0; i < n; i++) {
    float curStep = stepSizes[bestLevel[i]];
    if (direction[i]) {
      grid[i] += curStep;
    }
    else {
      grid[i] -= curStep;
    }
    smallest = min(smallest, grid[i]);
    largest = max(largest, grid[i]);
  }

  float range = (largest - smallest) / 2.0f;

  for (y = 0; y < HEIGHT; y++){
    for (x = 0; x < WIDTH; x++){
      grid[x+WIDTH*y] = ((grid[x+WIDTH*y] - smallest) / range) - 1.0f;
      uint8_t coll = 128 + (128 * grid[x+y*WIDTH]);
      display.writeFillRect(8*x, 8*y, 3, 3, RGB565(coll, coll, coll));
    }
  }
  
  display.display();

}

void rndrule(){

  base = randomf(1.5f, 2.4f);
  stepScale = randomf(0.01f, 0.2f);
  stepOffset = randomf(0.01f, 0.4f);
  blurFactor = randomf(0.5f, 1.0f);
  
  levels = (int) (logf(max(WIDTH,HEIGHT)) / logf(base)) - 1.0f;
  blurlevels = (int) max(0, (levels+1) * blurFactor - 0.5f);

  for (i = 0; i < levels; i++) {
    int radius = (int)powf(base, i);
    radii[i] = radius;
    stepSizes[i] = logf(radius) * stepScale + stepOffset;
  }

  for (i = 0; i < n; i++) grid[i] = randomf(-1.0f, 1.0f);

}
