// Worms cellular automata //

#include <M5GFX.h>
#include <M5AtomDisplay.h>

#define WIDTH   1280
#define HEIGHT  720
#define SCR     (WIDTH*HEIGHT)

M5AtomDisplay display (WIDTH, HEIGHT);

uint16_t size = ((2*WIDTH) * (2*HEIGHT));

#define NUMANTS 8

  uint16_t *world = NULL;
  uint16_t coll[NUMANTS];
  int x[NUMANTS];
  int y[NUMANTS];
  int antsdir[NUMANTS];


void rndrule(){

  display.fillScreen(TFT_BLACK);
  
  memset(world, 0, 4*SCR);

  for(int i = 0; i < NUMANTS; i++){
  
    x[i] = esp_random()%WIDTH;
    y[i] = esp_random()%HEIGHT;
    antsdir[i] = esp_random()%8;
    coll[i] = esp_random();
    
  }
         
}

static void on_button(void){ rndrule(); }

void setup(void){

  world = (uint16_t*)ps_malloc(size);

  srand(time(NULL));

  display.begin();
  display.startWrite();
  display.setColorDepth(16);
  display.fillScreen(TFT_BLACK);

  pinMode(39, INPUT);
  attachInterrupt(digitalPinToInterrupt(39), on_button, FALLING);

  rndrule();

}

void loop(void){
  
  for(int i = 0; i < NUMANTS; i++){
      
    if (world[x[i]+WIDTH*y[i]] > TFT_BLACK){ antsdir[i] = antsdir[i] - 1; world[x[i]+WIDTH*y[i]] = TFT_BLACK; }
    else { antsdir[i] = antsdir[i] + 1; world[x[i]+WIDTH*y[i]] = coll[i]; }

    if (antsdir[i] > 7) antsdir[i] = 0;   
    if (antsdir[i] < 0) antsdir[i] = 7;
    
    switch(antsdir[i]){
      case 0: y[i]--; break;
      case 1: y[i]--; x[i]++; break;
      case 2: x[i]++; break;
      case 3: x[i]++; y[i]++; break;
      case 4: y[i]++; break;
      case 5: y[i]++; x[i]--; break;
      case 6: x[i]--; break;
      case 7: x[i]--; y[i]--; break;
    }
    
    if (x[i] > WIDTH-1) x[i] = 0;
    if (x[i] < 0) x[i] = WIDTH-1;
    if (y[i] > HEIGHT-1) y[i] = 0;
    if (y[i] < 0) y[i] = HEIGHT-1;

    uint16_t image = world[x[i]+WIDTH*y[i]];  
    display.writePixel(x[i], y[i], image);
    
  }

}
