// Metaballs demo //

#include <M5AtomDisplay.h>

#define WIDTH   1280
#define HEIGHT  720

M5AtomDisplay display (WIDTH, HEIGHT);

#define NUM_BALLS     6
#define MIN_VELOCITY  2
#define MAX_VELOCITY  7
#define MIN_RADIUS    5
#define MAX_RADIUS    10

uint16_t coll;

struct vector2 {
    int16_t x;
    int16_t y;
};

struct ball {
    struct vector2 position;
    struct vector2 velocity;
    uint16_t radius;
};

struct ball balls[NUM_BALLS];

void metaballs_init(){

  display.fillScreen(TFT_BLACK);
  
  for (int i = 0; i < NUM_BALLS; i++) {
    balls[i].radius = (esp_random() % MAX_RADIUS) + MIN_RADIUS;
    balls[i].position.x = esp_random() % (WIDTH/6);
    balls[i].position.y = esp_random() % (HEIGHT/6);
    balls[i].velocity.x = (esp_random() % MAX_VELOCITY) + MIN_VELOCITY;
    balls[i].velocity.y = (esp_random() % MAX_VELOCITY) + MIN_VELOCITY;
  }
  
}

void metaballs_animate(){
  
  for (int i = 0; i < NUM_BALLS; i++) {
    balls[i].position.x += balls[i].velocity.x;
    balls[i].position.y += balls[i].velocity.y;
    if ((balls[i].position.x < 0) | (balls[i].position.x > (WIDTH/6))) balls[i].velocity.x = balls[i].velocity.x * -1;
    if ((balls[i].position.y < 0) | (balls[i].position.y > (HEIGHT/6))) balls[i].velocity.y = balls[i].velocity.y * -1;
  }
  
}

void metaballs_render(){  

  for (int y = 0; y < HEIGHT/6; y++) {
    for (int x = 0; x < WIDTH/6; x++) {
      float sum = 0;
      for (int i = 0; i < NUM_BALLS; i++) {
        float dx = x - balls[i].position.x;
        float dy = y - balls[i].position.y;
        float d2 = dx * dx + dy * dy;
        sum += balls[i].radius * balls[i].radius / d2;
      }
      
      if (sum > 0.75f) coll = TFT_BLUE;
      else if (sum > 0.5f) coll = TFT_WHITE;
      else if (sum > 0.35f) coll = TFT_YELLOW;
      else coll = TFT_BLACK;

      display.writeFillRect(6*x, 6*y, 3, 3, coll);
            
    }
  }
}

static void on_button(void){ metaballs_init(); }

void setup(void){

  srand(time(NULL));

  display.init();
  display.startWrite();
  display.setColorDepth(16);
  display.fillScreen(TFT_BLACK);

  pinMode(39, INPUT);
  attachInterrupt(digitalPinToInterrupt(39), on_button, FALLING);

  metaballs_init();

}

void loop(void){

  display.waitDisplay();
  
  metaballs_animate();
  metaballs_render();

  display.display();

}
