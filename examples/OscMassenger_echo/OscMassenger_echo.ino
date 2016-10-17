#include <OscMassenger.h>
OscMassenger msg;

void setup() {
  Serial.begin(57600); // Initiate serial communication.
}

void loop() {
  /*
    msg.sendBegin("/bob");
    msg.sendLong(millis());
    msg.sendFloat(4.5);
    msg.sendEnd();
    delay(1000);
  */
  while ( msg.receive() ) { // Check if there is a new massage
    msg.sendBegin("/ms");
    msg.sendLong(millis());
    msg.sendEnd();
    msg.dispatch("/led",ledMsg);
  }

  
}

void ledMsg() {
   msg.sendBegin("/echo");
    msg.sendInt(msg.nextInt());
    msg.sendInt(msg.nextInt());
    msg.sendEnd();
}
