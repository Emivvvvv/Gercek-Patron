#include <Arduino.h>
#include "SlaveduinoMovement.h"

void SlaveduinoMovement::waitStartSignal()
{
    while (receiveBuffer[0] == 0xf)
        readSerial();
}

void SlaveduinoMovement::init()
{
    Serial.begin(57600);
    waitStartSignal();
    sendArduinoInfo();
}

void SlaveduinoMovement::sendArduinoInfo()
{
    byte sensorduinoInfoBuffer[3] = {0x0D, 0x0, 0x0};
    Serial.write(sensorduinoInfoBuffer, 3);
}

void SlaveduinoMovement::run() {
    listenRaspberry();
    delay(7);
    listenRaspberry();
    delay(7);
    listenRaspberry();
    delay(7);
    sendSerial();
    delay(29);
}

// sends the sendBuffer to Raspberry Pi
void SlaveduinoMovement::sendSerial() {
    Serial.write(sendBuffer, SENDBUFFERSIZE);
}

void SlaveduinoMovement::setMovementData(uint16_t newPWM, bool newBrakeStatus, bool newLevitationStatus) {
    setInductionMotorPWM(newPWM);
    setBrakes(newBrakeStatus);
    setLevitation(newLevitationStatus);
}

// returns true if the receiveBuffer is changed after call,
// returns false if the receiveBuffer is not changed.
bool SlaveduinoMovement::readSerial()
{
    if (Serial.available() >= RECEIVEBUFFERSIZE)
    {
        Serial.readBytes(receiveBuffer, RECEIVEBUFFERSIZE);
        return true;
    }
    return false;
}

// message 		byte1(op)   byte2   byte3
//
// Sets the PWM of the induction motor.
// Byte2 and byte3 combined is the uint16 value.
// induction    0x01         uint16
//
// Sets the current status depending on the last bit.
// if the last bit is 0 sets brake/levitation to off
// if the last bit is 1 sets brake/levitation to on
// brake 	  	  0x0A      0x00 	  uint8
// levitation   0x0B      0x00 	  uint8
bool SlaveduinoMovement::messageEncoder()
{
    if (receiveBuffer[0] == 0x01) {
        uint16_t newPWM = (receiveBuffer[1] << 8) | receiveBuffer[2];
        setInductionMotorPWM(newPWM);
    } else {
        valueStatus = receiveBuffer[2] == 0x01;

        if (receiveBuffer[0] == 0x0A)
            {
                setBrakes(valueStatus);
            }
            else if (receiveBuffer[0] == 0x0B)
            {
                setLevitation(valueStatus);
            } else {
                return false;
            }
    }

    return true;
}

// if there is something coming from raspberry, executes it
// else nothing happens.
void SlaveduinoMovement::listenRaspberry()
{
    if (readSerial())
    {
        messageEncoder(); // Runs the command that raspberry sent.
    }
}

void SlaveduinoMovement::setInductionMotorPWM(uint16_t newPWM)
{
    uint8_t byte1 = (uint8_t)(newPWM >> 8);
    uint8_t byte2 = (uint8_t)(newPWM && 0x00FF);

    sendBuffer[0 + 0 * 3] = 0x01;
    sendBuffer[1 + 0 * 3] = byte1;
    sendBuffer[2 + 0 * 3] = byte2;
}

void SlaveduinoMovement::setBrakes(bool newStatus)
{
    sendBuffer[0 + 1 * 3] = 0x0A;
    sendBuffer[1 + 1 * 3] = 0x00;
    sendBuffer[2 + 1 * 3] = newStatus;
}

void SlaveduinoMovement::setLevitation(bool newStatus)
{
    sendBuffer[0 + 2 * 3] = 0x0B;
    sendBuffer[1 + 2 * 3] = 0x00;
    sendBuffer[2 + 2 * 3] = newStatus;
}

void SlaveduinoMovement::sendOperationSuccessful()
{
    byte sensorduinoInfoBuffer[3] = {0x0E, 0x0, 0x0};
    Serial.write(sensorduinoInfoBuffer, 3);
}

void SlaveduinoMovement::testRun()
{
    getTestData();
    for (int i = 0; i < 600; i++)
    {
        sendSerial();
        delay(55);
        listenRaspberry();
        delay(15);
        listenRaspberry();
        delay(15);
        listenRaspberry();
        delay(15);
    };
}

void SlaveduinoMovement::getTestData()
{
    sendBuffer[0 + 0 * 3] = 0x01;
    sendBuffer[1 + 0 * 3] = 0x00;
    sendBuffer[2 + 0 * 3] = 0x31;

    sendBuffer[0 + 1 * 3] = 0x0A;
    sendBuffer[1 + 1 * 3] = 0x00;
    sendBuffer[2 + 1 * 3] = 0x00; // currently not breaking!

    sendBuffer[0 + 2 * 3] = 0x0B;
    sendBuffer[1 + 2 * 3] = 0x00;
    sendBuffer[2 + 2 * 3] = 0x01; // Levitating (feat. DaBaby) - Dua Lipa
}
