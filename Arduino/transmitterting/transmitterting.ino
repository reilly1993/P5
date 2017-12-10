// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@open.com.au)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>

void setup()
{
    Serial.begin(9600);	  // Debugging only
    Serial.println("setup");

    vw_setup(2000);	 // Bits per sec
}

bool down = false;

void loop()
{
   //Serial.println(analogRead(A0));
    delay(100);
    if(analogRead(A0) == 0 && down != true) {
      Serial.println("click");
      const char *msg = "bitches all over my dick";
      vw_send((uint8_t *)msg, strlen(msg));
      vw_wait_tx(); // Wait until the whole message is gone
      delay(200);
      down = true;
    }

    if(analogRead(A0) == 0) return;
    
    down = false;
}
