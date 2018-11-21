
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

#define SS_PIN 53
#define RST_PIN 3 //write

#define DOORPIN 4
#define WINDOWPIN 5
#define LIGHTPIN 13

#define MOONBUTTONPIN 15
#define MOONLIGHTPIN 14

//RFID
#include <SPI.h>
#include <MFRC522.h>


char doorTriggers[3][10][5];
char lightTriggers[3][10][5];

#include "RFID_READ.h"
#include "RF_READ.h"

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

String drivewayActor = "";
bool isRaining = true;
bool isDay = true;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
 
void setup() 
{
  initTriggerArray(doorTriggers);
  //initTriggerArray(lightTriggers);
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  
  mfrc522.PCD_Init();   // Initiate MFRC522

  mfrc522.PCD_SetRegisterBitMask(mfrc522.RFCfgReg, (0x07<<4));

  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_rx_pin(2);
  vw_setup(2000);   // Bits per sec

  vw_rx_start();       // Start the receiver PLL running
  
  pinMode(DOORPIN, OUTPUT);
  pinMode(WINDOWPIN, OUTPUT);
  pinMode(LIGHTPIN, OUTPUT);

  pinMode(MOONBUTTONPIN, INPUT);
  pinMode(MOONLIGHTPIN, OUTPUT);

  digitalWrite(MOONLIGHTPIN, !isDay);
  //digitalWrite(SUNLIGHTPIN, isDay);

  parseBuffer("021-30-|415-40-51-||", 19);
  
  /*while(true) {
    readRFID(mfrc522, "driveway", &drivewayActor);
    readRF(); //updates triggers
    readDayState();

    if(check(doorTriggers)) {
      toggleDoor(true);
    } else {
      toggleDoor(false);
    }
  }*/
}


void loop() 
{
  /* READ RFID's and update actor variables */
  //readRFID(mfrc522, "driveway", &drivewayActor);
  //readRFID(mfrc522HOUSE, "house", $houseActor);

  /* READ wireless data and update trigger arrays */
//  readRF(); //updates triggers

  /* READ day and weather switches and update variables */
  //isRaining = digitalRead(something);
  //isDay = digitalRead(somethingelse);
  
  /* CHECK if something should be turned on based on variables and triggers */
  if(check(doorTriggers)) {
    //toggleGarage(garageServo, true);
  } else {
    //toggleGarage(garageServo, false);
  }
  
  if(check(lightTriggers)) {
    toggleLight(true);
  } else {
    toggleLight(false);
  }
  
  //delay(500);
}
int moonReading = LOW;
int sunReading = LOW;
int readState = true;

void readDayState() {
  moonReading = digitalRead(MOONBUTTONPIN);
  if(moonReading) {
    Serial.println(moonReading);
    readState = false;
  }

  if(readState != isDay) {
    isDay = readState;
    if(isDay) {
      digitalWrite(MOONLIGHTPIN, LOW);
      //digitalWrite(SUNLIGHTPIN, HIGH);
    } else {
      digitalWrite(MOONLIGHTPIN, HIGH);
      //digitalWrite(SUNLIGHTPIN, LOW);
    }
    
  }

  /*sunReading = digitalRead(SUNBUTTONPIN);
  if(sunReading) {
    Serial.println(sunReading);
    isDay = true;  
  }*/

  
}

bool check(char triggerArray[][10][5]) {
  for(int i = 0; i<3; i++) {
    if(triggerArray[i][0][0] != '\0') { //TODO: er -1 default? //only checks if something in row
      return checkRow(triggerArray[i]);
    }
  }
  return false;
}

bool checkRow(char (* row)[5]) {
  for(int i = 0; i<10; i++) {
    if(row[i][0] != '\0') { //not empty row
      if((String)row[i] == "20" && !isDay) return false;
      else if((String)row[i] == "21" && isDay) return false;
      
      /*switch(row[i]) {
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
      }*/
    }
  }
  return true;
}

bool lightOn = false;
bool windowOpen = false;
bool doorOpen = false;

void toggleLight(bool state) {
  if(state != lightOn) {
    digitalWrite(LIGHTPIN, state);
    Serial.print("Light is "); Serial.println(state ? "on": "off");
    lightOn = state;
  }
}

void toggleWindow(bool opened) {
  if(opened != windowOpen) {
    digitalWrite(WINDOWPIN, opened);
    Serial.print("Window is "); Serial.println(opened ? "open" : "closed");
    windowOpen = opened;
  }
}

void toggleDoor(bool opened) {
  if(opened != doorOpen) {
    digitalWrite(DOORPIN, opened);
    Serial.print("Door is "); Serial.println(opened ? "open" : "closed");
    doorOpen = opened;
  }
}

void initTriggerArray(char arr[][10][5]) {
  for(int i = 0; i<3; i++) {
    for(int j = 0; j<10; j++) {
      arr[i][j][0] = '\0';
    }
  }
}

void debug() {
  if (Serial.available() > 0) {
      char what = Serial.read();
      if(what) {
        Serial.println(what);
        if(what == 'a') {
          toggleWindow(true);
          toggleDoor(true);
          toggleLight(true);
        }
  
        if(what == 'b') {
          toggleWindow(false);
          toggleDoor(false);
          toggleLight(false);
        }
      }
    }
}
