
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
#define WINDOWRFIDPIN 48
#define HOUSERFIDPIN 49
#define RST_PIN 3 //write

#define DOORPIN 4
#define WINDOWPIN 5
#define LIGHTPIN 12

#define MOONBUTTONPIN 15
#define MOONLIGHTPIN 14
#define SUNBUTTONPIN 16
#define SUNLIGHTPIN 17

//RFID
#include <SPI.h>
#include <MFRC522.h>


char doorTriggers[3][10][5];
char windowTriggers[3][10][5];

#include "RFID_READ.h"
#include "RF_READ.h"

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
MFRC522 mfrc522Window(WINDOWRFIDPIN, RST_PIN);
MFRC522 mfrc522House(HOUSERFIDPIN, RST_PIN);

String doorActor = "";
String windowActor = "";
String houseActor = "";
bool isRaining = true;
bool isDay = false;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
 
void setup() 
{
  initTriggerArray(doorTriggers);
  initTriggerArray(windowTriggers);
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  
  mfrc522.PCD_Init();   // Initiate MFRC522
  mfrc522Window.PCD_Init();   // Initiate MFRC522
  mfrc522House.PCD_Init();   // Initiate MFRC522

  mfrc522.PCD_SetRegisterBitMask(mfrc522.RFCfgReg, (0x07<<4));
  mfrc522Window.PCD_SetRegisterBitMask(mfrc522Window.RFCfgReg, (0x07<<4));
  mfrc522House.PCD_SetRegisterBitMask(mfrc522House.RFCfgReg, (0x07<<4));

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
  pinMode(SUNLIGHTPIN, OUTPUT);
  pinMode(SUNBUTTONPIN, INPUT);

  digitalWrite(MOONLIGHTPIN, !isDay);
  digitalWrite(SUNLIGHTPIN, isDay);

  //parseBuffer("021-30-|415-40-51-||", 19);
  
  while(true) {
    
    readRFID(mfrc522, "door", &doorActor);
    readRFID(mfrc522Window, "window", &windowActor);
    readRFID(mfrc522House, "house", &houseActor);
    readRF(); //updates triggers
    readDayState();
    //debug();
    if(check(doorTriggers)) {
      toggleDoor(true);
    } else {
      toggleDoor(false);
    }

    if(check(windowTriggers)) {
      toggleWindow(true);
    } else {
      toggleWindow(false);
    }
  }
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
  
  /*if(check(lightTriggers)) {
    toggleLight(true);
  } else {
    toggleLight(false);
  }*/
  
  //delay(500);
}
int moonReading = LOW;
int sunReading = LOW;
int readState = false;

void readDayState() {
  
  moonReading = digitalRead(MOONBUTTONPIN);
  //Serial.println(moonReading);
  if(moonReading) {
    Serial.println(moonReading);
    readState = false;
  }

  sunReading = digitalRead(SUNBUTTONPIN);
  if(sunReading) {
    Serial.println(sunReading);
    readState = true;  
  }

  if(readState != isDay) {
    isDay = readState;
    if(isDay) {
      digitalWrite(MOONLIGHTPIN, LOW);
      digitalWrite(SUNLIGHTPIN, HIGH);
    } else {
      digitalWrite(MOONLIGHTPIN, HIGH);
      digitalWrite(SUNLIGHTPIN, LOW);
    }
    
  }

  

  
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
    String s = (String)row[i];
    if(row[i][0] != '\0') { //not empty row
      if(s == "10" && doorActor != "") return false;
      else if(s == "11" && doorActor == "") return false;
      else if(s == "20" && windowActor != "") return false;
      else if(s == "21" && windowActor == "") return false;
      else if(s == "30" && houseActor != "") return false;
        else if(s == "31" && houseActor == "") return false;
      else if(s == "40" && isDay) return false;
      else if(s == "41" && !isDay) return false;
      
      if(row[i][0] == '5')
        if(!checkActor(houseActor, row[i])) return false;
      
      if(row[i][0] == '6')
        if(!checkActor(doorActor, row[i])) return false;
      
      if(row[i][0] == '7')
        if(!checkActor(windowActor, row[i])) return false;
      
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

bool checkActor(String actor, char trigger[5]) {
  if(trigger[1] == '1') {
    if((trigger[2] == '3' && trigger[3] == '3') && actor != "boy") return false;
    if((trigger[2] == '3' && trigger[3] == '6') && actor != "girl") return false;
    if((trigger[2] == '2' && trigger[3] == '6') && actor != "cat") return false;
  }
  if(trigger[1] == '0') {
    if((trigger[2] == '3' && trigger[3] == '3') && actor == "boy") return false;
    if((trigger[2] == '3' && trigger[3] == '6') && actor == "girl") return false;
    if((trigger[2] == '2' && trigger[3] == '6') && actor == "cat") return false;
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
