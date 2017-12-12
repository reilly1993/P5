#include <VirtualWire.h>

void readRF() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  int (*triggerArrays)[10];
  if (vw_get_message(buf, &buflen)) {
    switch(buf[0]) { //which hub?
      case '0': //garage
        triggerArrays = garagedoorTriggers;
        break;
      case '1': //light
        triggerArrays = lightTriggers;
        break;
      //case '2': //door
    }
    //int i = 1, j = 0, k = 0;
    for (int i = 1, j = 0, k = 0; i < buflen; i++) {
      if(buf[i] == '|') {
        triggerArrays[j][k] = -1; //end
        j++; //next trigger array
        k = 0; //reset trigger index
      } else {
        triggerArrays[j][k] = buf[i];
        k++; //next trigger index
      }
    }
  }
}
