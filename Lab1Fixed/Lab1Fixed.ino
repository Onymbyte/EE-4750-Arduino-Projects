/*
 * Lab1
 * Author: Jameson Connor Harrington
 * Date: 2/9/22
 * 
 */
// variables for switching and trailing effect
int BASE = 2;               
int NUM = 12;
bool switcher = false;

void setup() {
  //order for trailing
  int LEDOrder[NUM] = {2,12,3,4,10,5,6,11,7,9,13,8};
  for (int i = BASE; i < BASE + NUM; i ++){
    pinMode(i, OUTPUT);      //set port ‘i’ as an output port 
  }
  
  for (int i = 0; i < NUM; i ++){
    digitalWrite(LEDOrder[i], HIGH);    // Turn ON the I/O board LED
    delay(200);             
  }
  for (int i = 0; i <  NUM; i ++){
    digitalWrite(LEDOrder[i], LOW);      // Turn OFF the I/O board LED           
  }
  //Starting lights
  digitalWrite(2, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(7, HIGH);
  delay(5000);
}

void loop() {
  if (switcher){
    //East-West Yellow and East-West red off
    digitalWrite(4, LOW);
    digitalWrite(6, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    //wait 1 second before yellow off
    delay(1000);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    //East and West Red
    digitalWrite(5, HIGH);
    digitalWrite(7, HIGH);
    
    //North and South Green
    digitalWrite(2, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(9, LOW);
  }
  else {
    //North and South Yellow then Red
    digitalWrite(2, LOW);
    digitalWrite(8, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    //North-South Red
    digitalWrite(3, HIGH);
    digitalWrite(9, HIGH);
    //East and West Green
    digitalWrite(4, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(7, LOW);
  }
  //wait 5 seconds
  delay(5000);
  //switch direction
  switcher = (!switcher);

}
