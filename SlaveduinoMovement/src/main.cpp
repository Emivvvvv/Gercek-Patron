#include <Arduino.h>
#include "SlaveduinoMovement.h"

void setup() {
    SlaveduinoMovement slaveduinoMovement = SlaveduinoMovement();

    slaveduinoMovement.init();
    delay(100);
    slaveduinoMovement.testRun();
    slaveduinoMovement.sendOperationSuccessful();
}

void loop() {
}