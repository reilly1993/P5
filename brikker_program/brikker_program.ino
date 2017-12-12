#include <SoftwareSerial.h>

SoftwareSerial portMale(8, 9);
SoftwareSerial portFemale(10, 11);

String readString;
String writeString;

bool upwards = true;
long int lastTime;

void setup() {
  pinMode(9, INPUT);
  Serial.begin(9600);

  if(!digitalRead(9)) delay(100); //dobbelt test
  upwards = digitalRead(9);
  
  if(!upwards) {
    portMale = SoftwareSerial(10, 11); //9 input
    portFemale = SoftwareSerial(3, 2); //10 output
  } else {
    portMale = SoftwareSerial(11, 10); //8 input
    portFemale = SoftwareSerial(2, 3); //11 output
  }

  portMale.begin(9600);
  portFemale.begin(9600);
}

void loop() {
  readString = portFemale.readStringUntil('|');
  writeString = upwards ? "11-" : "10-"; //[0] = identifier, [1] tilt
  if(readString.length() > 0) {
    writeString += readString + "|";
    portMale.print(writeString);
  } else {
    portMale.print(writeString + '|');
  }
  delay(10);
}
