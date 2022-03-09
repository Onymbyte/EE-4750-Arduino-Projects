
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
  if(newStr.length() <= 16){
    lcd.print(newStr);
    lcd.setCursor(0,1);
    lcd.print("EE4750 Section 3");
  }
  else if(newStr.length() <= 40){
    lcd.print(newStr);
    lcd.setCursor(0,1);
    lcd.print("EE4750 Section 3");
    for(int i = 0; i < newStr.length();i++){
      lcd.scrollDisplayLeft();
      delay(200);
    }
  }
  else {
    String tempStr = newStr;
    for(int i = 0; i < newStr.length() % 40; i++)
      tempStr += " ";
    for(int i = 0; i < newStr.length(); i++) {
      if(i == 0) {
        lcd.setCursor(0,1);
        lcd.print("EE4750 Section 3");
      }
      if(i % 40 == 0) {
        //lcd.clear();
        lcd.setCursor(0,0);
        Serial.println(i);
        Serial.println(tempStr.substring(i, i+40));
        lcd.print(tempStr.substring(i, i+40));
      }
      else if(i % 20 == 0){
        lcd.setCursor(0,0);
        Serial.println(i);
        Serial.println(tempStr.substring(i+20, i+40));
        lcd.print(tempStr.substring(i+20, i+40));
        
      }
      lcd.scrollDisplayLeft();
      delay(200);
    }
  }

}
//0000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999aaaaaaaaaabbbbbbbbbbcccccccc
  
