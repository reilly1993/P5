/* Til forloop for at læse nye tags
 Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
 Serial.print(mfrc522.uid.uidByte[i], HEX);
*/
//RFID
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
 
#define SS_PIN 10
#define RST_PIN 9 //write
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
Servo garageServo;
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  garageServo.attach(9);
  pinMode(8, OUTPUT);
}
void loop() 
{
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) 
  {
    String content= "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
       content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
       content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();
    readDrivewayRFID(content.substring(1));
    delay(1000);
  } else {
    garageServo.write(0);
    digitalWrite(8, LOW);
  }

  doStuff();
} 

void readDrivewayRFID(String RFID) {
  if (RFID == "05 EE F4 C2") {
      Serial.println("cat on driveway");
      garageServo.write(255);                  // sets the servo position according to the scaled value
    } else if (RFID == "3D 42 2E 72") {
      Serial.println("car_1 on driveway");
      digitalWrite(8, HIGH);
    } else {
      Serial.println("Not recognized");
    }
}

typedef struct Brick {
 bool on = false;
 int actor = 0;
};

Brick garageBricks[10];

void readRF(char hub, String definition) {
  int i = 0;
  String brickDef = "";
  int brickNum;
  int actorNum;
  //garageBricks = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
  while(getValue(definition, '|', i).length()>0) {
    brickDef = getValue(definition, '|', i);
    if(brickDef.substring(2,3) == ".") { //actor brick
      actorNum = brickDef.substring(3,5).toInt();
      brickNum = brickDef.substring(0,2).toInt();
    } else { //normal brick
      brickNum = brickDef.toInt();
    }

    if(actorNum>0) {
      garageBricks[brickNum].on = true;
      garageBricks[brickNum].actor = actorNum;
    } else {
      garageBricks[brickNum].on = true;
    }
  }
}

void doStuff() {
  
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

