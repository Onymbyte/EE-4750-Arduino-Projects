
#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


// Global variables for string and address management
char in_byte = 0;
String str = "";
int address = 0;

// Function that runs on startup
void setup() {
  // Begin LCD and Serial
  lcd.begin(16,2);
  Serial.begin(9600);
  // Display initial line and prompt user
  Serial.println("Please write your favorite line");
  displayStr("Scott & Jameson");
}

//Function that loops indefinitely while board runs
void loop() {
  // Wait for Serial port to have data
  if(Serial.available() > 0) {
    in_byte = Serial.read();      // Read character from serial port
    // Check for end of string or max character length
     if (in_byte != '\n' && str.length() < 128) {
      str += in_byte;
     }
     else {                       // Display string and write to EEPROM
      int len = str.length();     // Get length of string

      // Tell Serial port that the input has been received
      Serial.println("Thanks for your input. You have typed:");
      Serial.println(str);
      Serial.println(len);
      Serial.flush();             // Flush Serial port

      // Get random seed for random using analog and get random address in EEPROM
      randomSeed(analogRead(0));
      address = random(EEPROM.length() - str.length()-1);
      
      // Write the length and string into EEPROM
      EEPROM.write(address, str.length());
      for(int i=0; i < str.length(); i++){
        EEPROM.write(address+1+i, str[i]);
      }
      str = "";
      //Read from the EEPROM line by line
      for(int i = 0; i < len; i++) {
        char newChar = char(EEPROM.read(address+1+i));
        Serial.println(String(address+1+i) + " is the location for my input to store: " + newChar);
        str += newChar; 
      }
      //display the stored string
      Serial.println(str);
      displayStr(str);
      delay(3000);
      
      //Reset screen and display
      str = "";
      Serial.println("Please write your favorite line");
      displayStr("Scott & Jameson");
     }
  }
}

// Function for displaying a string to LCD string
void displayStr(String newStr) {
  // Clear screen and set cursor
  lcd.clear();
  lcd.setCursor(0,0);
  if(newStr.length() <= 16){      // Display string with 16 or less characters
    lcd.print(newStr);
    lcd.setCursor(0,1);
    lcd.print("EE4750 Section 3");
  } 
  else if(newStr.length() <= 40){ // 40 or less length string needs to only scroll
    lcd.print(newStr);
    lcd.setCursor(0,1);
    lcd.print("EE4750 Section 3");
    for(int i = 0; i < newStr.length() - 16 ;i++){
      lcd.scrollDisplayLeft();
      delay(500);
    }
  } 
  else {                          // For longer strings scroll around replace beginning of string while scrolling
    // Adding some padding for printing
    String tempStr = newStr;
    for(int i = 0; i < 40 - newStr.length() % 40; i++)
      tempStr += " ";
    // Loop through string while scrolling and replacing
    for(int i = 0; i < newStr.length() - 16; i++) {
      if(i == 0) {                // Move cursor and set initial bottom text
        lcd.setCursor(0,1);
        lcd.print("EE4750 Section 3");
      }
      if(i % 40 == 0) {           //Every 40 characters (length of buffer for top line)
        lcd.setCursor(0,0);
        lcd.print(tempStr.substring(i, i+40));
      }
      else if(i % 20 == 0){       //Every 20 characters replace the first 20 characters
        lcd.setCursor(0,0);
        lcd.print(tempStr.substring(i+20, i+40));
      }
      lcd.scrollDisplayLeft();
      delay(500);
    }
  }
}
// Test 128 character string
// 0000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999aaaaaaaaaabbbbbbbbbbcccccccc
  
