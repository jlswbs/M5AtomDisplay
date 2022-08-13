// Super Langton's Ant cellular automata //

#include <M5GFX.h>
#include <M5AtomDisplay.h>

#define WIDTH   1280
#define HEIGHT  720
#define SCR     (WIDTH*HEIGHT)

M5AtomDisplay display (WIDTH, HEIGHT);

uint16_t size = ((2*WIDTH) * (2*HEIGHT));

#define LENGHT  32

  uint8_t *world = NULL;
  int antX = WIDTH/2;
  int antY = HEIGHT/2;
  int direction;
  int stateCount;
  bool type[LENGHT];
  uint16_t stateCols[LENGHT];


void rndrule(){

  display.fillScreen(TFT_BLACK);

  memset(world, 0, 4*SCR);

  antX = WIDTH/2;
  antY = HEIGHT/2;

  stateCount = 2 + esp_random()%(LENGHT-2);
  direction = esp_random()%4;

  for(int i = 0; i < stateCount; i++) type[i] = esp_random()%2;
  for(int i = 0; i < stateCount; i++) stateCols[i] = esp_random();
         
}

void turn(int angle){
  
  if(angle == 0){
    if(direction == 0){
      direction = 3;
    } else {
      direction--;
    }
  } else {
    if(direction == 3){
      direction = 0;
    } else {
      direction++;
    }
  }
}

void move(){
  
  if(antY == 0 && direction == 0){
    antY = HEIGHT-1;
  } else {
    if(direction == 0 ){
      antY--;
    }
  }
  if(antX == WIDTH-1 && direction == 1){
    antX = 0;
  } else {
    if(direction == 1){
      antX++;
    }
  }
  if(antY == HEIGHT-1 && direction == 2){
   antY = 0; 
  } else {
    if(direction == 2){
      antY++;
    }
  }
  if(antX == 0 && direction == 3){
    antX = WIDTH-1;
  } else {
    if(direction == 3){
      antX--;
    }
  }
}

static void on_button(void){ rndrule(); }

void setup(void){

  world = (uint8_t*)ps_malloc(size);

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

  move();
    
  turn(type[(world[antX+antY*WIDTH]%stateCount)]);
  world[antX+antY*WIDTH]++;

  uint16_t image = stateCols[(world[antX+antY*WIDTH]%stateCount)];
  display.writePixel(antX, antY, image);
  
}
