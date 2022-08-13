// Turmites cellular automata //

#include <M5GFX.h>
#include <M5AtomDisplay.h>

#define WIDTH   1280
#define HEIGHT  720
#define SCR     (WIDTH*HEIGHT)

M5AtomDisplay display (WIDTH, HEIGHT);

uint16_t size = ((2*WIDTH) * (2*HEIGHT));

  uint8_t *world = NULL;
  uint16_t image;
  int x, y, posx, posy;
  int state, dir, moves;
  int filled;
  int last_filled;
  int current_diff;
  int current_col;
  int next_col[4][4];
  int next_state[4][4];
  int directions[4][4];


void rndrule(){

  display.fillScreen(TFT_BLACK);

  memset(world, 0, 4*SCR); 

  filled = 0;
  last_filled = 0;
  moves = 0;
  state = esp_random()%4;
  dir = esp_random()%8;
  posx = WIDTH/2;
  posy = HEIGHT/2;
  
  for(int j=0; j<4; j++){   
    for(int i=0; i<4; i++){         
      next_col[i][j] = esp_random()%4;
      next_state[i][j] = esp_random()%4;
      directions[i][j] = esp_random()%8;
    }   
  } 

  world[posx+posy*WIDTH] = esp_random()%4;
         
}

void move_turmite(){

  moves ++;
  
  int cols = world[posx+posy*WIDTH];

  if((cols > 0) && (next_col[cols][state] == 0)) filled--;
  if((cols == 0) && (next_col[cols][state] > 0)) filled++;
  
  if((moves % 100) == 0){
    current_diff = filled - last_filled;
    last_filled = filled;
  }
  
  x = posx;
  y = posy;
  current_col = next_col[cols][state];
  world[posx+posy*WIDTH] = next_col[cols][state];
  state = next_state[cols][state];    

  dir = (dir + directions[cols][state]) % 8;

  switch(dir){
    case 0: posy--; break;
    case 1: posy--; posx++; break;
    case 2: posx++; break;
    case 3: posx++; posy++; break;
    case 4: posy++; break;
    case 5: posy++; posx--; break;
    case 6: posx--; break;
    case 7: posx--; posy--; break;
  }

  if(posy < 0) posy = HEIGHT-1;
  if(posy >= HEIGHT) posy = 0;
  if(posx < 0) posx = WIDTH-1;
  if(posx >= WIDTH) posx=0;
  
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

  move_turmite();

  switch(current_col){
    case 0: image = TFT_RED; break;
    case 1: image = TFT_GREEN; break;
    case 2: image = TFT_BLUE; break;
    case 3: image = TFT_WHITE; break;
  }

  if((moves>1000) && (filled < 50)) rndrule();
  if((moves>5000) && (filled < 500) && (current_diff == 0)) rndrule();
  if((moves>20000) && (current_diff == 0) && (filled < 1000)) rndrule();

  display.writePixel(x, y, image);

}
