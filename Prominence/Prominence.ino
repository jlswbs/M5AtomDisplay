// Prominence math patterns //

#include <M5GFX.h>
#include <M5AtomDisplay.h>

#define WIDTH   1280
#define HEIGHT  720
#define SCR     (WIDTH*HEIGHT)

M5AtomDisplay display (WIDTH, HEIGHT);

uint16_t size = ((2*WIDTH) * (2*HEIGHT));

float randomf(float minf, float maxf) {return minf + (esp_random()%(1UL << 31)) * (maxf - minf) / (1UL << 31);}

#define N     1000
#define M     5
#define ITER  30

  uint16_t coll = esp_random();
  float h = 0.5f;
  float ds = -1.0f;
  float X[N];
  float Y[N];
  float T[M];
  float dx, dy, a, s, k;
  int ct;

void rndrule(){

  display.fillScreen(TFT_BLACK);
  
  for (int I = 0 ; I < N ; I++){
    X[I] = randomf(0.0f , WIDTH);
    Y[I] = WIDTH;
  }
  
  for (int II = 0 ; II < M ; II++) T[II] = randomf(10.0f, 40.0f);
         
}

static void on_button(void){ rndrule(); }

void setup(void){

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
  
  if(ct == ITER) {
    
    coll = esp_random();
    ct = 0;
    
  }    

  for (int I = 0; I < N; I++){

    for (int II = 0; II < M; II++){
      
      a = 2.0f * PI * II / M;
      k = (X[I] * cosf(a)) - ((Y[I]+s) * sinf(a));
      k = h * sinf(k/T[II]);
      float kdx = -k * sinf(-a);
      float kdy = +k * cosf(-a);
      dx = dx + kdx;
      dy = dy + kdy;
      
    }
    
    X[I] = X[I] + dx;
    Y[I] = Y[I] + dy;
    dx = 0;
    dy = 0;
    
    if(X[I] < 0 || X[I] > WIDTH || Y[I] > HEIGHT || Y[I] < 0 || randomf(0.0f, 2000.0f) < 1) {
      
      Y[I] = 0.0f;
      X[I] = randomf(0.0f, WIDTH);   
    
    }
    
    a = PI * X[I] / WIDTH;
    k = Y[I] + 10.0f;

    int ax = (WIDTH/2) + (k * sinf(a));
    int bx = (WIDTH/2) - (k * sinf(a));    
    int ay = (HEIGHT/2) + (k * cosf(a));
    
    if(ax>0 && ax<WIDTH && ay>0 && ay<HEIGHT) {      
    
      display.writePixel(ax, ay, coll);
      display.writePixel(bx, ay, coll);
    
    }
  
  }
  
  s = s + ds;
  ct++;

}
