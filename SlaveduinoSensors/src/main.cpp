#include <Arduino.h>
#include "SlaveduinoSensor.h"

void setup() {
    SlaveduinoSensor slaveduinoSensor = SlaveduinoSensor();

    slaveduinoSensor.init();
    delay(100);
    slaveduinoSensor.testRun();
    slaveduinoSensor.sendOperationSuccessful();
}

void loop() {
}