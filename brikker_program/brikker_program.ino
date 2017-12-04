#include <SoftwareSerial.h>

SoftwareSerial portMale(8, 9);
SoftwareSerial portFemale(10, 11);

String readString;
String writeString;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);

  if(!digitalRead(2)) {
    portMale = SoftwareSerial(8, 9); //9 input
    portFemale = SoftwareSerial(10, 11); //10 output
  } else {
    portMale = SoftwareSerial(10, 11); //11 input
    portFemale = SoftwareSerial(8, 9); //8 output
  }

  while(!Serial);

  portMale.begin(9600);
  portFemale.begin(9600);
}

void loop() {
  readString = portFemale.readStringUntil('|');
  writeString = digitalRead(2) ? "AC1-" : "AC0-";
  
  if(readString.length() > 0){
    Serial.println(readString);
    writeString += readString + "|";
    portMale.print(writeString);
  } else {
    portMale.print(writeString + '|');
  }
}
