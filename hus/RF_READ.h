#include <VirtualWire.h>

void clearTrigger(char* trigger) {
  for(int i = 0; i<5; i++) {
    trigger[i] = '\0';
  }
}

void parseBuffer(uint8_t* buf, uint8_t buflen) {
  char (*triggerArrays)[10][5];
  
  switch(buf[0]) { //which hub?
    case '0': //garage
      triggerArrays = doorTriggers;
      break;
    case '1': //light
      triggerArrays = windowTriggers;
      break;
    //case '2': //door
  }
  //int i = 1, j = 0, k = 0;
  char trigger[5];
  clearTrigger(trigger); //reset to \0
  for (int i = 1, j = 0, k = 0, l = 0; i < buflen; i++) {
    if(buf[i] == '|') {
      triggerArrays[j][k][0] = '\0'; //end of side
      j++; //next trigger array
      k = 0; //reset trigger index
    } else {
      //Serial.println((char)buf[i]);
      if((char)buf[i] == '-') {
        trigger[l] = '\0'; //end of string
        strcpy(triggerArrays[j][k], trigger);
        //Serial.println(trigger);
        k++; //next trigger index
        l = 0; //reset trigger string index
        clearTrigger(trigger); //reset trigger string
      } else {
        trigger[l] = buf[i];
        l++;
      }
    }
  }

  /*for(int i = 0; i<3; i++) {
    if(triggerArrays[i][0][0] != '\0') {
      for(int j = 0; j<10; j++) {
        if(triggerArrays[i][j][0] != '\0') {
          Serial.print(i);Serial.print("-");
          Serial.println(triggerArrays[i][j]);
          Serial.println((String)triggerArrays[i][j] == "21");
        }
      }
    }
  }*/
}

void readRF() {
  //Serial.println("HEY");
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) {
    Serial.println((char*)buf); //received string
    parseBuffer(buf, buflen);
  }
}
