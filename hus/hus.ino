
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

#define SS_PIN 10
#define RST_PIN 9 //write

//RFID
#include <SPI.h>
#include <MFRC522.h>
#include <ServoTimer2.h>

int garagedoorTriggers[3][10];
int lightTriggers[3][10]; //TODO initialize to -1

#include "RFID_READ.h"
#include "RF_READ.h"

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
ServoTimer2 garageServo;
String drivewayActor = "";
bool isRaining = true;
bool isDay = true;
 
void setup() 
{
  initTriggerArray(garagedoorTriggers);
  initTriggerArray(lightTriggers);
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  
  mfrc522.PCD_Init();   // Initiate MFRC522
  
  garageServo.attach(9);
  
  pinMode(8, OUTPUT); //LIGHT in HOUSE
}


void loop() 
{
  /* READ RFID's and update actor variables */
  readRFID(mfrc522, "driveway", &drivewayActor);
  //readRFID(mfrc522HOUSE, "house", $houseActor);

  /* READ wireless data and update trigger arrays */
  readRF(); //updates triggers

  /* READ day and weather switches and update variables */
  //isRaining = digitalRead(something);
  //isDay = digitalRead(somethingelse);
  
  /* CHECK if something should be turned on based on variables and triggers */
  if(check(garagedoorTriggers)) {
    toggleGarage(garageServo, true);
  } else {
    toggleGarage(garageServo, false);
  }
  
  if(check(lightTriggers)) {
    toggleLight(8, true);
  } else {
    toggleLight(8, false);
  }
  
  //delay(500);
}

bool check(int triggerArray[][10]) {
  for(int i = 0; i<3; i++) {
    if(triggerArray[i][0] != -1) { //TODO: er -1 default? //only checks if something in row
      return checkRow(triggerArray[i]);
    }
  }
  return false;
}

bool checkRow(int * row) {
  for(int i = 0; i<10; i++) {
    if(row[i] != -1) {
      switch(row[i]) {
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
  }
  return true;
}

void toggleLight(int pin, bool state) {
  digitalWrite(pin, state);
  Serial.print("Light set "); Serial.println(state);
}

void toggleWindow(ServoTimer2 s, bool opened) {
  s.write(opened ? 255 : 0);
  Serial.print("Window set "); Serial.println(opened);
}

void toggleGarage(ServoTimer2 s, bool opened) {
  s.write(opened ? 255 : 0);
  //Serial.print("Garage set "); Serial.println(opened);
}

void initTriggerArray(int arr[][10]) {
  for(int i = 0; i<3; i++) {
    for(int j = 0; j<10; j++) {
      arr[i][j] = -1;
    }
  }
}

