/*
 * Author: Scott Boudreaux
 * Lab Partner: Jameson Connor Harrington
 */
// initialize the library with the numbers of the interface pins
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
  
int brightnessPin = 9;
int pMatrixPin = 10;
  
void setup()                                                                                     
{
// set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  pinMode(brightnessPin, OUTPUT);
  pinMode(pMatrixPin, OUTPUT);
}

void loop()
{
  analogWrite(brightnessPin, 255);
  analogWrite(pMatrixPin, 100);
  
// Print a message to the LCD.
  lcd.setCursor(3,0);  
  lcd.print("Go Tigers!");
  lcd.setCursor(4,1);
  lcd.print("Go LSU!");
  delay(2000);

for (int positionCounter = 0; positionCounter < 1; positionCounter++) 
    {
    // scroll one position right:
    lcd.scrollDisplayRight();
    delay(2000);
    }

// Clear display
  lcd.clear();
  delay(2000);

  lcd.setCursor(3,0);  
  lcd.print("Go Tigers!");
  lcd.setCursor(4,1);
  lcd.print("Go LSU!");
  delay(2000);

for (int positionCounter = 0; positionCounter < 1; positionCounter++) 
    {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    delay(2000);
    }
  

// Clear display
  lcd.clear();
  delay(2000);

  lcd.setCursor(3,0);  
  lcd.print("Go Tigers!");
  lcd.setCursor(4,1);
  lcd.print("Go LSU!");
  delay(2000);

for (int brightnessCounter = 255; brightnessCounter > 0; brightnessCounter = brightnessCounter - 10) 
    {
    analogWrite(brightnessPin, brightnessCounter);
    delay(100);
    }

  analogWrite(brightnessPin, 255);
  lcd.setCursor(3,0);  
  lcd.print("Go Tigers!");
  lcd.setCursor(4,1);
  lcd.print("Go LSU!");
  delay(2000);

 for (int pixelCounter = 100; pixelCounter < 150; pixelCounter = pixelCounter + 5) 
    {
    analogWrite(pMatrixPin, pixelCounter);
    delay(150);
    }

 for (int positionCounter = 0; positionCounter < 1; positionCounter++) 
    {
    // scroll one position right:
    lcd.scrollDisplayRight();
    delay(2000);
    }

    lcd.clear();
    delay(500);
}
