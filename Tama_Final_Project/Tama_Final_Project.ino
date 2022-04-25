#include <Arduino_MKRIoTCarrier.h>
#include <arduino-timer.h>
#include "bitmaps.h"

MKRIoTCarrier carrier;

uint8_t hunger = 150;
uint8_t happiness = 150;
int training;
int discipline;
unsigned long myTime = 0;
int age;
float temperature;
bool on;
unsigned long pressTime;
bool offPress = false;
auto timer = timer_create_default();
uint16_t background = ST77XX_WHITE;
bool eating = false;



void setup() {
  Serial.begin(9600);

  CARRIER_CASE = false;
  pinMode(TFT_BACKLIGHT, OUTPUT);
  carrier.begin();
  initDisplay();

  
  on = true;

  timer.every(5000, updateTemperature);
  timer.every(100, updateStat);

}

void loop() {
  timer.tick();
  carrier.Buttons.update();
  
  if(carrier.Button2.onTouchChange()){
    if(offPress){
      if (pressTime + 1000 <= millis()){
        on = !on;
        digitalWrite(TFT_BACKLIGHT, on);
      }
      offPress = !offPress;
    }
    else {
      pressTime = millis();
      //Serial.println(pressTime);
      offPress = !offPress;
    }
  }
  if (on) {
    //statusBar(70, 70, ST77XX_ORANGE, "Hunger", hunger);
    //statusBar(70, 90, ST77XX_YELLOW, "Happiness", happiness);
    //carrier.display.fillCircle(120, 120, 115, ST77XX_BLACK);
    uint16_t size = 96;
    carrier.display.fillRect(120-size/2, 120-size/2, size, size, ST7735_BLUE);
    if(carrier.Button3.onTouchDown() && !eating){
      feed();
    }
 
  }
  

  
}

void initDisplay() {
  carrier.display.setRotation(0);
  carrier.display.fillScreen(background);
  statusBar(70, 170, ST77XX_ORANGE, "Hunger", hunger);
  statusBar(70, 190, ST77XX_YELLOW, "Happiness", happiness);
}

void statusBar(int x, int y, uint16_t color, String nameOf, int percentage) {
  int width = 102;
  int height = 10;
  
  carrier.display.setCursor(x, y);
  carrier.display.setTextSize(height/10);
  carrier.display.setTextColor(ST77XX_BLACK);
  carrier.display.print(nameOf);
  
  carrier.display.drawRect(x,y+height,width,height,ST77XX_BLACK);
  carrier.display.fillRect(x+1, y+height+1, 100, height-2, background);
  if (percentage <= 100)
    carrier.display.fillRect(x+1, y+height+1, percentage, height-2, color);
  else carrier.display.fillRect(x+1, y+height+1, 100, height-2, color);

  carrier.display.fillRect(x+width+1, y+height+1, 50, height, background);
  carrier.display.setCursor(x+width+1, y+height+1);
  carrier.display.print(String(percentage)+"%");
  //carrier.display.print(String(percentage)+"/100");
}

bool updateTemperature(void *) {
  temperature = carrier.Env.readTemperature(FAHRENHEIT);
  carrier.display.setCursor(70, 30);
  carrier.display.fillRect(70, 30, 50, 10, background);
  carrier.display.print(temperature);
  return true;
}

bool updateStat(void *) {
  if (hunger > 0) {
    hunger--;
  }
  if (happiness > 0) {
    if (hunger < 33 and happiness > 1 or hunger >= 110)
      happiness -= 2;
    else {
      happiness--;
    }
      
  }
  statusBar(70, 170, ST77XX_ORANGE, "Hunger", hunger);
  statusBar(70, 190, ST77XX_YELLOW, "Happiness", happiness);

  
  return true;
}
void feed() {
  eating = true;
  carrier.display.drawRGBBitmap(180, 100, myBitmap, 32, 32);
  timer.in(500, [] (void *) -> bool {carrier.display.fillRect(180, 100, 8, 32, background);});
  timer.in(1000, [] (void *) -> bool {carrier.display.fillRect(180, 100, 16, 32, background);});
  timer.in(1500, [] (void *) -> bool {carrier.display.fillRect(180, 100, 24, 32, background);});
  timer.in(2000, [] (void *) -> bool {
    carrier.display.fillRect(180, 100, 32, 32, background);
    hunger += 30;
    eating = false;
    });
}
