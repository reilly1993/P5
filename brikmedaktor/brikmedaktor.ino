#include <SoftwareSerial.h>

//male: 8, 9
//female: 6, 7

SoftwareSerial portMale(8, 9);
SoftwareSerial portFemale(6, 7);

String readString;
String writeString;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);

  if(!digitalRead(2)) {
    portMale = SoftwareSerial(8, 9); //9 input
    portFemale = SoftwareSerial(6, 7); //10 output
  } else {
    portMale = SoftwareSerial(9, 8); //8 input
    portFemale = SoftwareSerial(7, 6); //11 output
  }

  while(!Serial);

  portMale.begin(9600);
  portFemale.begin(9600);
}

void loop() {
  readString = portFemale.readStringUntil('|');
  writeString = digitalRead(2) ? "AC1" : "AC0";
  writeString += ".";
  writeString += analogRead(A5);
  writeString += "-";
  
  if(readString.length() > 0){
    Serial.println(readString);
    writeString += readString + "|";
    portMale.print(writeString);
  } else {
    portMale.print(writeString + '|');
  }
}
