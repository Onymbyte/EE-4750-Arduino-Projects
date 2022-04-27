/*
Dog sprites were made by Dawn for Stardew Valley at this link:
https://community.playstarbound.com/threads/fluffy-dogs-other-alternative-dog-sprites-update-8-pug-time.109948/

Pork Chop Icon:
<a href="https://www.flaticon.com/free-icons/beef" title="beef icons">Beef icons created by Freepik - Flaticon</a>

Tennis Ball Icon:
<a href="https://www.flaticon.com/free-icons/ball" title="ball icons">Ball icons created by Freepik - Flaticon</a>

*/
#include <Arduino_MKRIoTCarrier.h>
#include <arduino-timer.h>
#include "bitmaps.h"

MKRIoTCarrier carrier;

uint8_t hunger = 100;
uint8_t happiness = 100;
int training;
int discipline;
unsigned long lastActionTime = 0;
int age;
float temperature;
bool on;
unsigned long pressTime;
bool offPress = false;
auto timer = timer_create_default();
uint16_t background = 0xAF9F;
uint16_t lower_background = 0x5E4B;
bool eating = false;
// Dog state by index [sit, sad, sleep, eating, running]
bool prevDogState [] = {true, false, false, false, false};
bool dogState [] = {true, false, false, false, false};



void setup() {
  Serial.begin(9600);

  CARRIER_CASE = false;
  pinMode(TFT_BACKLIGHT, OUTPUT);
  carrier.begin();
  initDisplay();
  
  on = true;

  timer.every(5000, updateTemperature);
  timer.every(1000, updateStat);

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
    
    if(carrier.Button3.onTouchDown() && (dogState[0] || dogState[2])){
      feed();
    }
    if(carrier.Button4.onTouchDown() && (dogState[0] || dogState[2])){
      play();
    }
 
  }
  

  
}

void initDisplay() {
  carrier.display.setRotation(0);
  carrier.display.fillScreen(background);
  carrier.display.fillRect(0, 140,240,100, lower_background);
  statusBar(70, 170, ST77XX_ORANGE, "Hunger", hunger);
  statusBar(70, 190, ST77XX_YELLOW, "Happiness", happiness);
  carrier.display.drawRGBBitmap(120-42/2, 120-66/2, dog_tailWag1_42_66, 42,66);
  timer.every(10000, sit0);
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

  carrier.display.fillRect(x+width+1, y+height+1, 50, height, lower_background);
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
  lastActionTime = millis();
  eating = true;
  for (int i=0; i<5; i++)
    dogState[i] = false;
  dogState[3] = true;
  carrier.display.drawRGBBitmap(168, 100, steak, 32, 32);
  clearPetArea();
  carrier.display.drawRGBBitmap(120-72/2, 120-66/2, dog_eating0_72_66, 72, 66);
  timer.in(500, [] (void *) -> bool {
    carrier.display.fillRect(168, 100, 8, 32, background);
    clearPetArea();
    carrier.display.drawRGBBitmap(120-72/2, 120-66/2, dog_eating1_72_66, 72, 66);
    });
  timer.in(1000, [] (void *) -> bool {
    carrier.display.fillRect(168, 100, 16, 32, background);
    clearPetArea();
    carrier.display.drawRGBBitmap(120-72/2, 120-66/2, dog_eating0_72_66, 72, 66);
    });
  timer.in(1500, [] (void *) -> bool {
    carrier.display.fillRect(168, 100, 24, 32, background);
    clearPetArea();
    carrier.display.drawRGBBitmap(120-72/2, 120-66/2, dog_eating1_72_66, 72, 66);
    });
  timer.in(2000, [] (void *) -> bool {
    carrier.display.fillRect(168, 100, 32, 32, background);
    clearPetArea();
    carrier.display.drawRGBBitmap(120-42/2, 120-66/2, dog_tailWag0_42_66, 42, 66);
    hunger = min(hunger+30, 125);
    for (int i=0; i<5; i++)
      dogState[i] = false;
    dogState[0] = true;
    });
}
void play() {
  lastActionTime = millis();
  for (int i=0; i<5; i++)
    dogState[i] = false;
  dogState[4] = true;
  carrier.display.drawRGBBitmap(180, 140, ball, 32, 32);
  clearPetArea();
  carrier.display.drawRGBBitmap(120-90/2, 120-63/2, dog_run0_90_63, 90, 63);
  timer.in(750, [] (void *) -> bool {
    happiness = min(happiness+10,100);
    clearPetArea();
    carrier.display.drawRGBBitmap(120-90/2, 120-63/2, dog_run1_90_63, 90, 63);
    });
  timer.in(750*2, [] (void *) -> bool {
    happiness = min(happiness+10,100);
    clearPetArea();
    carrier.display.drawRGBBitmap(120-90/2, 120-63/2, dog_run2_90_63, 90, 63);
    });
  timer.in(750*3, [] (void *) -> bool {
    happiness = min(happiness+10,100);
    clearPetArea();
    carrier.display.drawRGBBitmap(120-90/2, 120-63/2, dog_run0_90_63, 90, 63);
    });
  timer.in(750*4, [] (void *) -> bool {
    carrier.display.fillRect(180, 140, 32, 32, lower_background);
    clearPetArea();
    carrier.display.drawRGBBitmap(120-42/2, 120-66/2, dog_tailWag0_42_66, 42, 66);
    for (int i=0; i<5; i++)
      dogState[i] = false;
    dogState[0] = true;
    });
}

void clearPetArea(){
  carrier.display.fillRect(120-96/2, 120-96/2,96,68, background);
  carrier.display.fillRect(120-96/2, 140,96,18, lower_background);
}
bool sit0(void *){
  if(dogState[0]){
    if(millis() - lastActionTime > 5000){
      dogState[0] = false;
      dogState[2] = true;
      clearPetArea();
      carrier.display.drawRGBBitmap(120-78/2, 120-34/2, dog_sleeping_78_54, 78,54);
    }
    else {
      clearPetArea();
      carrier.display.drawRGBBitmap(120-42/2, 120-66/2, dog_tailWag0_42_66, 42,66);
    }
  }
  if(dogState[2] && millis() - lastActionTime <= 5000){
    dogState[2] = false;
    dogState[0] = true;
  }
  return true;
}
