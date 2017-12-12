#include <SoftwareSerial.h>

SoftwareSerial portMale(8, 9);
SoftwareSerial portFemale(6, 7);

String readString;
String writeString;

bool upwards = true;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);

  if(!digitalRead(9)) delay(100); //dobbelt test
  upwards = digitalRead(9);
  
  if(!upwards) {
    portMale = SoftwareSerial(2, 3); //9 input
    portFemale = SoftwareSerial(11, 10); //10 output
  } else {
    portMale = SoftwareSerial(3, 2); //8 input
    portFemale = SoftwareSerial(10, 11); //11 output
  }

  while(!Serial);

  portMale.begin(9600);
  portFemale.begin(9600);
}

void loop() {
  readString = portFemale.readStringUntil('|');
  writeString = upwards ? "01" : "00"; //[0] = identifier, [1] tilt
  writeString += (int) map(analogRead(A5), 0, 255, 0, 1000)/100; //0-5 (aktør)
  writeString += "-"; //ex: 003 = brik 0, tilt off, aktør 3
  if(readString.length() > 0){
    writeString += readString + "|";
    portMale.print(writeString);
  } else {
    portMale.print(writeString + '|');
  }
  delay(10);
}
