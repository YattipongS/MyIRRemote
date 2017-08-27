/*
 * IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 * Example circuit diagram:
 *  https://github.com/markszabo/IRremoteESP8266/wiki#ir-receiving
 * Changes:
 *   Version 0.2 June, 2017
 *     Changed GPIO pin to the same as other examples.
 *     Used our own method for printing a uint64_t.
 *     Changed the baud rate to 115200.
 *   Version 0.1 Sept, 2015
 *     Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009
 */
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <irRecv.h>
#include <IRsend.h>
#include <IRutils.h>

// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
// http://nodemcu.readthedocs.io/en/dev/en/modules/gpio/
uint16_t RECV_PIN = D4;
uint16_t SEND_PIN = D5;

IRrecv irrecv(RECV_PIN);

IRsend irsend(SEND_PIN); //an IR led is connected to GPIO pin 0

decode_results results;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.begin(115200);
  irrecv.enableIRIn();  // Start the receiver
  irsend.begin(); // start sender
}

void loop() {
  
  IR_Receive();

  IR_Send();
  
  delay(100);  
}

void IR_Receive()
{
  // Receive
  if (irrecv.decode(&results)) {    
    // print() & println() can't handle printing long longs. (uint64_t)
     if (results.decode_type == NEC) {
      Serial.print("NEC: ");
    } else if (results.decode_type == SONY) {
      Serial.print("SONY: ");
    } else if (results.decode_type == RC5) {
      Serial.print("RC5: ");
    } else if (results.decode_type == RC6) {
      Serial.print("RC6: ");
    } else if (results.decode_type == UNKNOWN) {
      Serial.print("UNKNOWN: ");
      Serial.print(results->address, HEX);
    }
    
    Serial.println("IR value:");
    serialPrintUint64(results.value, HEX);
    Serial.println("");    
    
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    
    irrecv.resume();  // Receive the next value
  }
}

byte incomingByte;
uint16_t buf[] = { 450,-450,450,-450,450,-450,500,-450,450,-450,450,-450,450,-450,450,-450,1300,-450,1300,-450,450,-450,450,-450,500,-450,450,-450,450,-450 };
void IR_Send()
{
    // Send data only when you receive data:
    if (Serial.available() > 0) {
      // do send
      Serial.println("Send Something");
      
      // read the incoming byte:
      incomingByte = Serial.read();
      delay(2000);
      
      // say what you got:
      Serial.print("I received: ");
      Serial.println(incomingByte, DEC);
      delay(2000);

      // send
      // Raw (31): 450 -450 450 -450 450 -450 500 -450 450 -450 450 -450 450 -450 450 -450 1300 -450 1300 -450 450 -450 450 -450 500 -450 450 -450 450 -450 
      
      irsend.sendRaw(buf, 31, 38);
      delay(2000);
      irsend
    }
        
}


