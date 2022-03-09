
#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

char in_byte = 0;
String str = "";
int counter = 0;
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  Serial.begin(9600);
  Serial.println("Please write your favorite line");
  displayStr("Scott & Jameson");

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0) {
    in_byte = Serial.read();
     if (in_byte != '\n' && str.length() < 128) {
      str += in_byte;
     }
     else {
      Serial.println("Thanks for your input. You have typed");
      Serial.println(str);
      Serial.println(str.length());
      Serial.flush();
      
      displayStr(str);
      
      str = "";
     }
  }
}
void displayStr(String newStr) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(newStr);
  lcd.setCursor(0,1);
  lcd.print("EE4750 Section 3");
}
