#include <Arduino.h>

const int ITEMCOUNT = 9; // total message that could be sent
const int SENDBUFFERSIZE = ITEMCOUNT * 3; // Buffer array size.
const int RECEIVEBUFFERSIZE = 3; // Buffer array size.

byte sendBuffer[SENDBUFFERSIZE]; // Buffer for sending data to Raspberry Pi
byte receiveBuffer[RECEIVEBUFFERSIZE]; // Buffer for receiving data from Raspberry Pi

uint16_t valueUi16;
float valueFloat32;
bool valueStatus;

void testRun();
void getDataFromSensors();
void getDataFromSensors2();
void sendSerial();
void sendArduinoInfo();
bool readSerial();
void setInductionMotorPWM(uint16_t);
void setBrakes(bool);
void setLevitation(bool);
bool messageEncoder();
void sendOperationSuccessfull();
void waitStartSignal();

void setup() {
  Serial.begin(115200);
  waitStartSignal();
  sendArduinoInfo();
  //run()
  testRun();
  sendOperationSuccessfull();
}

void loop() {
}

void testRun() {
  for (int i = 0; i < 1000; i++) {
    getDataFromSensors();
    sendSerial();
    getDataFromSensors2();
    sendSerial();
  };
}

void getDataFromSensors() {
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

void getDataFromSensors2() {
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

void waitStartSignal() {
  while (receiveBuffer[0] == 0xf) readSerial();
}

void sendArduinoInfo() {
    byte sensorduinoInfoBuffer[3] = {0x0C, 0x0, 0x0};
    Serial.write(sensorduinoInfoBuffer, 3);
}

// sends the sendBuffer to Raspberry Pi
void sendSerial() {
  Serial.write(sendBuffer, SENDBUFFERSIZE);
}

// returns true if the receiveBuffer is changed after call,
// returns false if the receiveBuffer is not changed.
bool readSerial() {
  if (Serial.available() >= RECEIVEBUFFERSIZE) {
      Serial.readBytes(receiveBuffer, RECEIVEBUFFERSIZE);
      return true;
  }
  return false;
}

void sendOperationSuccessfull() {
    byte sensorduinoInfoBuffer[3] = {0x0E, 0x0, 0x0};
    Serial.write(sensorduinoInfoBuffer, 3);
}