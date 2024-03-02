#include <Arduino.h>

const int ITEMCOUNT = 9; // total message that could be sent
const int SENDBUFFERSIZE = ITEMCOUNT * 3; // Buffer array size.
const int RECEIVEBUFFERSIZE = 3; // Buffer array size.

byte sendBuffer[SENDBUFFERSIZE]; // Buffer for sending data to Raspberry Pi
byte receiveBuffer[RECEIVEBUFFERSIZE]; // Buffer for receiving data from Raspberry Pi

uint16_t valueUi16;
float valueFloat32;
bool valueStatus;

void getDataFromSensors();
void sendSerial();
bool readSerial();
void setInductionMotorPWM(uint16_t);
void setBrakes(bool);
void setLevitation(bool);
bool messageEncoder();


void setup() {
  Serial.begin(115200);
  while (receiveBuffer[0] == 0xf) {readSerial();}
  sendBuffer[0] = 0x0C;
  sendSerial();
  getDataFromSensors();
  sendSerial();
}

void loop() {
}

void getDataFromSensors() {
  sendBuffer[0 + 0 * 3] = 0x00;
  sendBuffer[1 + 0 * 3] = 0x03;
  sendBuffer[2 + 0 * 3] = 0x01;

  sendBuffer[0 + 1 * 3] = 0x02;
  sendBuffer[1 + 1 * 3] = 0b00000011; // 3 decimal
  sendBuffer[2 + 1 * 3] = 0b00000001; // 1 deccimal

  sendBuffer[0 + 2 * 3] = 0x03;
  sendBuffer[1 + 2 * 3] = 0b00000110; // 6 decimal
  sendBuffer[2 + 2 * 3] = 0b00000010; // 2 deccimal

  sendBuffer[0 + 3 * 3] = 0x04;
  sendBuffer[1 + 3 * 3] = 0b00001001; // 9 decimal
  sendBuffer[2 + 3 * 3] = 0b00000011; // 3 deccimal

  sendBuffer[0 + 4 * 3] = 0x05;
  sendBuffer[1 + 4 * 3] = 0b00000011; // 3 decimal
  sendBuffer[2 + 4 * 3] = 0b00000001; // 1 deccimal

  sendBuffer[0 + 5 * 3] = 0x06;
  sendBuffer[1 + 5 * 3] = 0b00000110; // 6 decimal
  sendBuffer[2 + 5 * 3] = 0b00000010; // 2 deccimal

  sendBuffer[0 + 6 * 3] = 0x07;
  sendBuffer[1 + 6 * 3] = 0b00000011; // 3 decimal
  sendBuffer[2 + 6 * 3] = 0b00000001; // 1 deccimal

  sendBuffer[0 + 7 * 3] = 0x08;
  sendBuffer[1 + 7 * 3] = 0b00000110; // 6 decimal
  sendBuffer[2 + 7 * 3] = 0b00000010; // 2 deccimal

  sendBuffer[0 + 8 * 3] = 0x09;
  sendBuffer[1 + 8 * 3] = 0b00001001; // 9 decimal
  sendBuffer[2 + 8 * 3] = 0b00000011; // 3 deccimal
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

