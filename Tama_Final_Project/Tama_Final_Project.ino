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
  
  if(carrier.Buttons.onTouchChange(TOUCH2)){
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
    carrier.display.drawBitmap(120, 120, myBitmap, 32, 32, ST77XX_BLACK);
  }
  

  
}

void initDisplay() {
  carrier.display.setRotation(0);
  carrier.display.fillScreen(ST77XX_WHITE);
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
  carrier.display.fillRect(x+1, y+height+1, 100, height-2, ST77XX_WHITE);
  if (percentage <= 100)
    carrier.display.fillRect(x+1, y+height+1, percentage, height-2, color);
  else carrier.display.fillRect(x+1, y+height+1, 100, height-2, color);

  carrier.display.fillRect(x+width+1, y+height+1, 50, height, ST77XX_WHITE);
  carrier.display.setCursor(x+width+1, y+height+1);
  carrier.display.print(String(percentage)+"%");
  //carrier.display.print(String(percentage)+"/100");
}

bool updateTemperature(void *) {
  temperature = carrier.Env.readTemperature(FAHRENHEIT);
  carrier.display.setCursor(70, 30);
  carrier.display.fillRect(70, 30, 50, 10, ST77XX_WHITE);
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
