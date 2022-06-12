// Multi Langton's Ant cellular automata //

#include <M5AtomDisplay.h>

#define WIDTH   1280
#define HEIGHT  720
#define SCR     (WIDTH*HEIGHT)

M5AtomDisplay display (WIDTH, HEIGHT);

uint16_t size = ((2*WIDTH) * (2*HEIGHT));

#define NUMANTS 6

  uint16_t *world = NULL;
  uint16_t coll[NUMANTS];
  int x[NUMANTS];
  int y[NUMANTS];
  int antsdir[NUMANTS];


void rndrule(){

  display.fillScreen(TFT_BLACK);

  for(int i = 0; i < SCR; i++) world[i] = 0;

  for(int i = 0; i < NUMANTS; i++){
  
    x[i] = esp_random()%WIDTH;
    y[i] = esp_random()%HEIGHT;
    antsdir[i] = esp_random()%4;
    coll[i] = esp_random();
    
  }
         
}

static void on_button(void){ rndrule(); }

void setup(void){

  world = (uint16_t*)ps_malloc(size);

  srand(time(NULL));

  pinMode(39, INPUT);
  attachInterrupt(digitalPinToInterrupt(39), on_button, FALLING);
  
  display.begin();
  display.setColorDepth(16);
  display.fillScreen(TFT_BLACK);

  rndrule();

}

void loop(void){

  for(int i = 0; i < NUMANTS; i++){
    
    if (world[x[i]+WIDTH*y[i]] > 0){
      
      antsdir[i] = antsdir[i] - 1;
      world[x[i]+WIDTH*y[i]] = 0;
      
    } else {
      
      antsdir[i] = antsdir[i] + 1;
      world[x[i]+WIDTH*y[i]] = coll[i];
      
    }

    if (antsdir[i] > 3) antsdir[i] = 0;   
    if (antsdir[i] < 0) antsdir[i] = 3;   
    
    if (antsdir[i] == 0) x[i] = x[i] - 1;
    if (antsdir[i] == 1) y[i] = y[i] + 1;
    if (antsdir[i] == 2) x[i] = x[i] + 1;
    if (antsdir[i] == 3) y[i] = y[i] - 1;
    
    if (x[i] > WIDTH-1) x[i] = 0;
    if (x[i] < 0) x[i] = WIDTH-1;
    if (y[i] > HEIGHT-1) y[i] = 0;
    if (y[i] < 0) y[i] = HEIGHT-1;

    uint16_t image = world[x[i]+WIDTH*y[i]];  
    display.writePixel(x[i], y[i], image);
    
  }

}
