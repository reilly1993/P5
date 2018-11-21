#include <VirtualWire.h>
#include <SoftwareSerial.h>

SoftwareSerial ports[3] = {
  SoftwareSerial(10,11),
  SoftwareSerial(8,9),
  SoftwareSerial(6,7)
};

String readString;
String writeString;

void setup() {
  
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);

  vw_setup(2000);   // Bits per sec
  vw_set_tx_pin(2);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  for(int i = 0; i<3; i++) {
    ports[i].begin(9600);
    ports[i].setTimeout(300);
  }
}

String strings[3] = {"|", "|", "|"};
String readStrings[3] = {"", "", ""};

char buf[50];

void loop() {
  for(int i = 0; i<3; i++) {
    ports[i].listen();
    delay(20);
    readStrings[i] = ports[i].readStringUntil('|');
    readStrings[i] += '|'; //tilføj divider

    if(doIBelieveThis(readStrings[i])) {
      Serial.println(readStrings[i]);//debugging
    }
    
    if(readStrings[i] != strings[i]) { //tjek om en af de 3 statements har ændret sig
      strings[i] = readStrings[i];
      Serial.print("send: ");
      String sendString = "0"; //hub identitet  
      for(int i = 0; i<3; i++) {
        sendString += strings[i];
      }
      Serial.println(sendString);
      sendString.toCharArray(buf, 50); //konverter til char array for at kunne sende over 433mhz
      vw_send((uint8_t *)buf, strlen(buf));
      vw_wait_tx(); // Wait until the whole message is gone
      //delay(200); //nødvendigt?
    }
  }
}

int doIBelieveThis(String s) {
  return 1;
}
