#include <Arduino_MKRIoTCarrier.h>
#include <arduino-timer.h>

MKRIoTCarrier carrier;

uint16_t hunger = 50;
uint16_t happiness = 50;
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
  carrier.begin();
  pinMode(TFT_BACKLIGHT, OUTPUT);
  carrier.display.setRotation(0);
  carrier.display.fillScreen(ST77XX_CYAN);

  timer.every(5000, updateTemperature);
  on = true;
}

void loop() {
  timer.tick();
  carrier.Buttons.update();
  if(carrier.Buttons.onTouchChange(TOUCH2)){
    if(offPress){
      if (pressTime + 1000 <= millis()){
        Serial.println("hello");
        on = !on;
        digitalWrite(TFT_BACKLIGHT, on);
      }
      offPress = !offPress;
    }
    else {
      pressTime = millis();
      Serial.println(pressTime);
      offPress = !offPress;
    }
  }
  if (on) {
    statusBar(70, 70, ST77XX_ORANGE, "Hunger", hunger);
    statusBar(70, 90, ST77XX_YELLOW, "Happiness", happiness);
    carrier.display.fillCircle(120, 120, 115, ST77XX_BLACK);
  }
  
  delay(100);
  
}

void statusBar(int x, int y, uint16_t color, String nameOf, int percentage) {
  int width = 102;
  int height = 10;
  
  carrier.display.setCursor(x, y);
  carrier.display.setTextSize(height/10);
  carrier.display.setTextColor(ST77XX_BLACK);
  carrier.display.print(nameOf);
  
  carrier.display.drawRect(x,y+height,width,height,ST77XX_BLACK);
  if (percentage <= 100)
    carrier.display.fillRect(x+1, y+height+1, percentage, height-2, color);
  else carrier.display.fillRect(x+1, y+height+1, 100, height-2, color);
  
  carrier.display.setCursor(x+width+1, y+height+1);
  carrier.display.print(String(percentage)+"/100");
}

bool updateTemperature(void *) {
  temperature = carrier.Env.readTemperature();
  return true;
}
