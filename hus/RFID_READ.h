#include "DEFINITIONS.h"

long int lastSet = 0;

String getUID(byte *buffer, byte bufferSize) {
  String content = "";
  for (byte i = 0; i < bufferSize; i++) {
    content.concat(String(buffer[i] < 0x10 ? " 0" : " "));
    content.concat(String(buffer[i], HEX));
  }
  content.toUpperCase();
  return content.substring(1);
}

void readRFID(MFRC522 sensor, String pos, String* actorContainer) {
  sensor.PICC_ReadCardSerial();
  sensor.PICC_IsNewCardPresent();

  // Select one of the cards
  if (!sensor.PICC_ReadCardSerial()) {
    *actorContainer = "";
    return;
  }

  lastSet = millis();

  String UID = getUID(sensor.uid.uidByte, sensor.uid.size);

  for(int i = 0; i<ACTORS; i++) {
    if (UID == actorIDs[i]) {
      Serial.print(actorNames[i]); Serial.print(" on "); Serial.println(pos);
      *actorContainer = actorNames[i];
    }
  }
}


