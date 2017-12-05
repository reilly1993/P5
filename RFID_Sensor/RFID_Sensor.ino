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

#define ACTORS 2
String actorIDs[] = {"05 EE F4 C2", "3D 42 2E 72"};
String actorNames[] = {"cat", "car_1"};
String drivewayTrigger = "cat";


String drivewayActor = "";
bool isRaining = false;
bool isDay = false;
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  garageServo.attach(9);
  pinMode(8, OUTPUT);
}

bool card_present = false;

void loop() 
{
  //readRFID(mfrc522, "driveway", &drivewayActor);
  if(drivewayTrigger == drivewayActor) {
    toggleGarage(garageServo, true);
  } else {
    toggleGarage(garageServo, false);
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    card_present = false;
    Serial.println("no card");
  } else {
    card_present = true; 
  }
  
  delay(500);
} 

void readRFID(MFRC522 sensor, String pos, String* actorContainer) {
  if (sensor.PICC_IsNewCardPresent()) {
    if (sensor.PICC_ReadCardSerial()) {
      *actorContainer = "";
      String content= "";
      for (byte i = 0; i < sensor.uid.size; i++) {
         content.concat(String(sensor.uid.uidByte[i] < 0x10 ? " 0" : " "));
         content.concat(String(sensor.uid.uidByte[i], HEX));
      }
      content.toUpperCase();
      for(int i = 0; i<ACTORS; i++) {
        if (content.substring(1) == actorIDs[i]) {
          Serial.print(actorNames[i]); Serial.print(" on "); Serial.println(pos);
          *actorContainer = actorNames[i];
        }
      }
    }
  } else {
    *actorContainer = "";
  }
}

void toggleLight(int pin, bool state) {
  digitalWrite(pin, state);
  Serial.print("Light set "); Serial.println(state);
}

void toggleWindow(Servo s, bool opened) {
  s.write(opened ? 255 : 0);
  Serial.print("Window set "); Serial.println(opened);
}

void toggleGarage(Servo s, bool opened) {
  s.write(opened ? 255 : 0);
  Serial.print("Garage set "); Serial.println(opened);
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
