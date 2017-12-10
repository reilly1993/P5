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

String strings[3];
String first;
String second;
String third;

char buf[50];

void loop() {
  portFemale1.listen();
  first = softSerialEvent(portFemale1);
  if(first != strings[0]) {
    strings[0] = first;
    char* blah = first.toCharArray(buf, 50); //PROBLEM: omdanner bare string til char array. lav kopi som omdannes ;)
    vw_send((uint8_t *)blah, strlen(blah));
    vw_wait_tx(); // Wait until the whole message is gone
    delay(200);
    
  }
  delay(5);

  portFemale2.listen();
  second = softSerialEvent(portFemale2);
  if(second != strings[1]) {
    strings[1] = second;
  }
  delay(5);

  portFemale3.listen();
  third = softSerialEvent(portFemale3);
  if(third != strings[2]) {h
    strings[2] = third;
  }
  delay(5);

  Serial.println();
}

String softSerialEvent(SoftwareSerial &port) {
  readString = port.readStringUntil('|');
  readString += '|';
  Serial.println(readString);

  return readString;
}

