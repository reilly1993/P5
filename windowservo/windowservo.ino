/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int val;    // variable to read the value from the analog pin

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  for(int i = 100; i>1; i--) {
    myservo.write(i);
    delay(50);
  }
  delay(1000);                           // waits for the servo to get there
  for(int i = 0; i<100; i++) {
    myservo.write(i);
    delay(50);
  }
  
  delay(1000);
}

