
/* Til forloop for at læse nye tags
 Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
 Serial.print(mfrc522.uid.uidByte[i], HEX);
*/

/*

Definitioner:
0 = israining
1 = notraining
2 = day
3 = notday
4 = cat in driveway
5 = car_1 in driveway

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

String garagedoorTrigger = "cat";
int lightTriggers[] = {5, 0, 2};


String drivewayActor = "";
bool isRaining = true;
bool isDay = true;
 
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
  readRFID(mfrc522, "driveway", &drivewayActor);
  if(garagedoorTrigger == drivewayActor) {
    toggleGarage(garageServo, true);
  } else {
    toggleGarage(garageServo, false);
  }
  
  if(check(lightTriggers, 3)) {
    toggleLight(8, true);
  } else {
    toggleLight(8, false);
  }
  
  //delay(500);
}

bool check(int * triggerArray, int size) {
  for(int i = 0; i<size; i++) {
    switch(triggerArray[i]) {
      case 0:
        if(!isRaining) return false; break;
      case 1:
        if(isRaining) return false; break;
      case 2:
        if(!isDay) return false; break;
      case 3:
        if(isDay) return false; break;
      case 4:
        if(drivewayActor != "cat") return false; break;
      case 5:
        if(drivewayActor != "car_1") return false; break;
    }
  }

  return true;
}

long int lastSet = 0;

void readRFID(MFRC522 sensor, String pos, String* actorContainer) {
  mfrc522.PICC_ReadCardSerial();
  mfrc522.PICC_IsNewCardPresent();

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    *actorContainer = "";
    return;
  }

  lastSet = millis();

  String UID = getUID(mfrc522.uid.uidByte, mfrc522.uid.size);

  for(int i = 0; i<ACTORS; i++) {
    if (UID == actorIDs[i]) {
      Serial.print(actorNames[i]); Serial.print(" on "); Serial.println(pos);
      *actorContainer = actorNames[i];
    }
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
  //Serial.print("Garage set "); Serial.println(opened);
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

String getUID(byte *buffer, byte bufferSize) {
  String content = "";
  for (byte i = 0; i < bufferSize; i++) {
    content.concat(String(buffer[i] < 0x10 ? " 0" : " "));
    content.concat(String(buffer[i], HEX));
  }
  content.toUpperCase();
  return content.substring(1);
}
