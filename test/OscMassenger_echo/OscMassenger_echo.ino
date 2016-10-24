#include <OscMassenger.h>
OscMassenger msg;

void setup() {
  Serial.begin(57600); // Initiate serial communication.
}

void loop() {

  while ( msg.receive() ) { // Check if there is a new massage
    msg.sendBegin("/ms");
    msg.sendLong(millis());
    msg.sendEnd();
    msg.dispatch("/echo",echoMsg);
  }

  
}

void echoMsg() {
   msg.sendBegin("/echo");
    msg.sendInt(msg.nextInt());
    msg.sendFloat(msg.nextFloat());
    msg.sendEnd();
}