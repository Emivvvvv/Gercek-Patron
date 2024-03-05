#include <Arduino.h>
#include "SlaveduinoSensor.h"

void SlaveduinoSensor::waitStartSignal()
{
    while (receiveBuffer[0] == 0xf)
        readSerial();
}

void SlaveduinoSensor::init()
{
    Serial.begin(57600);
    waitStartSignal();
    sendArduinoInfo();
}

void SlaveduinoSensor::sendArduinoInfo()
{
    byte sensorduinoInfoBuffer[3] = {0x0C, 0x0, 0x0};
    Serial.write(sensorduinoInfoBuffer, 3);
}

void SlaveduinoSensor::run()
{
    //NOT IMPLEMENTED YET!
}

// void setSensorData(uint16_t, float, float, float, float, float, float, float, float) {
// };

// returns true if the receiveBuffer is changed after call,
// returns false if the receiveBuffer is not changed.
bool SlaveduinoSensor::readSerial()
{
    if (Serial.available() >= RECEIVEBUFFERSIZE)
    {
        Serial.readBytes(receiveBuffer, RECEIVEBUFFERSIZE);
        return true;
    }
    return false;
}

void SlaveduinoSensor::sendOperationSuccessful()
{
    byte sensorduinoInfoBuffer[3] = {0x0E, 0x0, 0x0};
    Serial.write(sensorduinoInfoBuffer, 3);
}

// sends the sendBuffer to Raspberry Pi
void SlaveduinoSensor::sendSerial()
{
    Serial.write(sendBuffer, SENDBUFFERSIZE);
}

void SlaveduinoSensor::testRun()
{
    for (int i = 0; i < 100; i++)
    {
        getTestData();
        sendSerial();
        delay(100);
        getTestData2();
        sendSerial();
        delay(100);
    };
}

void SlaveduinoSensor::getTestData()
{
    sendBuffer[0 + 0 * 3] = 0x00;
    sendBuffer[1 + 0 * 3] = 0x03;
    sendBuffer[2 + 0 * 3] = 0x01;

    sendBuffer[0 + 1 * 3] = 0x02;
    sendBuffer[1 + 1 * 3] = 0b00000011; // 3 decimal
    sendBuffer[2 + 1 * 3] = 0b00000001; // 1 decimal

    sendBuffer[0 + 2 * 3] = 0x03;
    sendBuffer[1 + 2 * 3] = 0b00000110; // 6 decimal
    sendBuffer[2 + 2 * 3] = 0b00000010; // 2 decimal

    sendBuffer[0 + 3 * 3] = 0x04;
    sendBuffer[1 + 3 * 3] = 0b00001001; // 9 decimal
    sendBuffer[2 + 3 * 3] = 0b00000011; // 3 decimal

    sendBuffer[0 + 4 * 3] = 0x05;
    sendBuffer[1 + 4 * 3] = 0b00000011; // 3 decimal
    sendBuffer[2 + 4 * 3] = 0b00000001; // 1 decimal

    sendBuffer[0 + 5 * 3] = 0x06;
    sendBuffer[1 + 5 * 3] = 0b00000110; // 6 decimal
    sendBuffer[2 + 5 * 3] = 0b00000010; // 2 decimal

    sendBuffer[0 + 6 * 3] = 0x07;
    sendBuffer[1 + 6 * 3] = 0b00000011; // 3 decimal
    sendBuffer[2 + 6 * 3] = 0b00000001; // 1 decimal

    sendBuffer[0 + 7 * 3] = 0x08;
    sendBuffer[1 + 7 * 3] = 0b00000110; // 6 decimal
    sendBuffer[2 + 7 * 3] = 0b00000010; // 2 decimal

    sendBuffer[0 + 8 * 3] = 0x09;
    sendBuffer[1 + 8 * 3] = 0b00001001; // 9 decimal
    sendBuffer[2 + 8 * 3] = 0b00000011; // 3 decimal
}

void SlaveduinoSensor::getTestData2()
{
    sendBuffer[0 + 0 * 3] = 0x00;
    sendBuffer[1 + 0 * 3] = 0x06;
    sendBuffer[2 + 0 * 3] = 0x02;

    sendBuffer[0 + 1 * 3] = 0x02;
    sendBuffer[1 + 1 * 3] = 0b00000011; // 3 decimal
    sendBuffer[2 + 1 * 3] = 0b00000001; // 1 decimal

    sendBuffer[0 + 2 * 3] = 0x03;
    sendBuffer[1 + 2 * 3] = 0b00000110; // 6 decimal
    sendBuffer[2 + 2 * 3] = 0b00000010; // 2 decimal

    sendBuffer[0 + 3 * 3] = 0x04;
    sendBuffer[1 + 3 * 3] = 0b00000011; // 3 decimal
    sendBuffer[2 + 3 * 3] = 0b00000001; // 1 decimal

    sendBuffer[0 + 4 * 3] = 0x05;
    sendBuffer[1 + 4 * 3] = 0b00000011; // 3 decimal
    sendBuffer[2 + 4 * 3] = 0b00000001; // 1 decimal

    sendBuffer[0 + 5 * 3] = 0x06;
    sendBuffer[1 + 5 * 3] = 0b00000110; // 6 decimal
    sendBuffer[2 + 5 * 3] = 0b00000010; // 2 decimal

    sendBuffer[0 + 6 * 3] = 0x07;
    sendBuffer[1 + 6 * 3] = 0b00000011; // 3 decimal
    sendBuffer[2 + 6 * 3] = 0b00000001; // 1 decimal

    sendBuffer[0 + 7 * 3] = 0x08;
    sendBuffer[1 + 7 * 3] = 0b00000110; // 6 decimal
    sendBuffer[2 + 7 * 3] = 0b00000010; // 2 decimal

    sendBuffer[0 + 8 * 3] = 0x09;
    sendBuffer[1 + 8 * 3] = 0b00000011; // 3 decimal
    sendBuffer[2 + 8 * 3] = 0b00000001; // 1 decimal
}