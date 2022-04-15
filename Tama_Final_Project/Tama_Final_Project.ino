#include <Arduino_MKRIoTCarrier.h>
#include <arduino-timer.h>

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

// 'beef', 32x32px
const unsigned char myBitmap [] PROGMEM = {
  0x00, 0x03, 0xc0, 0x00, 0x00, 0x06, 0x70, 0x00, 0x00, 0x06, 0x0c, 0x00, 0x00, 0x0e, 0x06, 0x00, 
  0x00, 0x1a, 0x03, 0x00, 0x00, 0x13, 0x00, 0xc0, 0x00, 0x31, 0x0c, 0x60, 0x00, 0x31, 0x8f, 0x20, 
  0x00, 0x70, 0xc9, 0x90, 0x00, 0x70, 0x44, 0x98, 0x00, 0x50, 0x26, 0xcc, 0x00, 0x58, 0x11, 0xc4, 
  0x00, 0xc8, 0x18, 0x02, 0x00, 0xc0, 0x04, 0x02, 0x01, 0xe0, 0x03, 0x03, 0x01, 0x20, 0x01, 0x81, 
  0x03, 0xb0, 0x00, 0xe1, 0x06, 0xd0, 0x80, 0x3f, 0x0e, 0x70, 0xc0, 0x0e, 0x0b, 0x00, 0x60, 0x18, 
  0x09, 0x80, 0x20, 0x30, 0x08, 0xc0, 0x21, 0xc0, 0x08, 0x60, 0x3f, 0x00, 0x0c, 0x38, 0x70, 0x00, 
  0x0e, 0x0e, 0xc0, 0x00, 0x7b, 0x03, 0x00, 0x00, 0xc1, 0x86, 0x00, 0x00, 0x83, 0xfc, 0x00, 0x00, 
  0xc2, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00
};



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
