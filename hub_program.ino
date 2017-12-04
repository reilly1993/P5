#include <SoftwareSerial.h>

SoftwareSerial portFemale1(10, 11);
SoftwareSerial portFemale2(8, 9);
SoftwareSerial portFemale3(6, 7);

String readString;
String writeString;

void setup() {
  
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  
  portFemale1.begin(9600);
  portFemale2.begin(9600);
  portFemale3.begin(9600);
}

void loop() {
  
  //if(portFemale1.read() != -1) {
    portFemale1.listen();
    softSerialEvent(portFemale1);
    delay(5);
  //}

  //if(portFemale2.read() != -1) {
    portFemale2.listen();
    softSerialEvent(portFemale2);
    delay(5);
  //}

  //if(portFemale3.read() != -1) {
    portFemale3.listen();
    softSerialEvent(portFemale3);
    delay(5);
  //}

  Serial.println();
}

int softSerialEvent(SoftwareSerial &port)
{
  readString = port.readStringUntil('|');
  if (readString.length() > 0) {
    Serial.println(readString);
  }

   return readString[3] == 'B';
}

