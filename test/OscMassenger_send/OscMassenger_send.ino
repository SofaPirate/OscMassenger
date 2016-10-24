#include <OscMassenger.h>
OscMassenger msg;

void setup() {
  Serial.begin(57600); // Initiate serial communication.
}

void loop() {
  msg.sendBegin("/bob");
  msg.sendLong(millis());
  msg.sendFloat(4.5);
  msg.sendEnd();
  delay(1000);
}
