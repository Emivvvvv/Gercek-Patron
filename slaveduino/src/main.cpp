#include <Arduino.h>

const int POSSIBLEMESSAGECOUNT = 12; // total message that could be sent
const int SENDBUFFERSIZE = POSSIBLEMESSAGECOUNT * 3; // Buffer array size.
const int RECEIVEBUFFERSIZE = 3; // Buffer array size.

byte sendBuffer[SENDBUFFERSIZE]; // Buffer for sending data to Raspberry Pi
byte receiveBuffer[RECEIVEBUFFERSIZE]; // Buffer for receiving data from Raspberry Pi

uint16_t valueUi16;
float valueFloat32;
bool valueStatus;

void getDataFromSensors();
void getDataFromSensors2();
void listenRaspberry();
void sendSerial();
bool readSerial();
void setInductionMotorPWM(uint16_t);
void setBrakes(bool);
void setLevitation(bool);
bool messageEncoder();


void setup() {
  Serial.begin(115200);
  getDataFromSensors();
  while (receiveBuffer[0] == 0xf) {readSerial();}
  sendSerial();
  delay(50);
  getDataFromSensors2();
  listenRaspberry();
  sendSerial();
  delay(50);
  listenRaspberry();
  sendSerial();
  delay(50);
  listenRaspberry();
  sendSerial();
}

void loop() { 
}

void getDataFromSensors() {
  sendBuffer[0 + 0x0 * 3] = 0x00;
  sendBuffer[1 + 0x0 * 3] = 0x03;
  sendBuffer[2 + 0x0 * 3] = 0x01;

  sendBuffer[0 + 0x1 * 3] = 0x01;
  sendBuffer[1 + 0x1 * 3] = 0x03;
  sendBuffer[2 + 0x1 * 3] = 0x01;

  sendBuffer[0 + 0x2 * 3] = 0x02;
  sendBuffer[1 + 0x2 * 3] = 0b00000011; // 3 decimal
  sendBuffer[2 + 0x2 * 3] = 0b00000001; // 1 deccimal

  sendBuffer[0 + 0x3 * 3] = 0x03;
  sendBuffer[1 + 0x3 * 3] = 0b00000110; // 6 decimal
  sendBuffer[2 + 0x3 * 3] = 0b00000010; // 2 deccimal

  sendBuffer[0 + 0x4 * 3] = 0x04;
  sendBuffer[1 + 0x4 * 3] = 0b00001001; // 9 decimal
  sendBuffer[2 + 0x4 * 3] = 0b00000011; // 3 deccimal

  sendBuffer[0 + 0x5 * 3] = 0x05;
  sendBuffer[1 + 0x5 * 3] = 0b00000011; // 3 decimal
  sendBuffer[2 + 0x5 * 3] = 0b00000001; // 1 deccimal

  sendBuffer[0 + 0x6 * 3] = 0x06;
  sendBuffer[1 + 0x6 * 3] = 0b00000110; // 6 decimal
  sendBuffer[2 + 0x6 * 3] = 0b00000010; // 2 deccimal

  sendBuffer[0 + 0x7 * 3] = 0x07;
  sendBuffer[1 + 0x7 * 3] = 0b00000011; // 3 decimal
  sendBuffer[2 + 0x7 * 3] = 0b00000001; // 1 deccimal

  sendBuffer[0 + 0x8 * 3] = 0x08;
  sendBuffer[1 + 0x8 * 3] = 0b00000110; // 6 decimal
  sendBuffer[2 + 0x8 * 3] = 0b00000010; // 2 deccimal

  sendBuffer[0 + 0x9 * 3] = 0x09;
  sendBuffer[1 + 0x9 * 3] = 0b00001001; // 9 decimal
  sendBuffer[2 + 0x9 * 3] = 0b00000011; // 3 deccimal

  sendBuffer[0 + 0xA * 3] = 0x0A;
  sendBuffer[1 + 0xA * 3] = 0x00;
  sendBuffer[2 + 0xA * 3] = 0x00; // currently not breaking!
  
  sendBuffer[0 + 0xB * 3] = 0x0B;
  sendBuffer[1 + 0xB * 3] = 0x00;
  sendBuffer[2 + 0xB * 3] = 0x01; // Levitating (feat. DaBaby) - Dua Lipa
}

void getDataFromSensors2() {
  sendBuffer[0 + 0x0 * 3] = 0x00;
  sendBuffer[1 + 0x0 * 3] = 0x31;
  sendBuffer[2 + 0x0 * 3] = 0x31;

  sendBuffer[0 + 0x1 * 3] = 0x01;
  sendBuffer[1 + 0x1 * 3] = 0x03;
  sendBuffer[2 + 0x1 * 3] = 0x01;

  sendBuffer[0 + 0x2 * 3] = 0x02;
  sendBuffer[1 + 0x2 * 3] = 0b00000011; // 3 decimal
  sendBuffer[2 + 0x2 * 3] = 0b00000001; // 1 deccimal

  sendBuffer[0 + 0x3 * 3] = 0x03;
  sendBuffer[1 + 0x3 * 3] = 0b00000110; // 6 decimal
  sendBuffer[2 + 0x3 * 3] = 0b00000010; // 2 deccimal

  sendBuffer[0 + 0x4 * 3] = 0x04;
  sendBuffer[1 + 0x4 * 3] = 0b00001001; // 9 decimal
  sendBuffer[2 + 0x4 * 3] = 0b00000011; // 3 deccimal

  sendBuffer[0 + 0x5 * 3] = 0x05;
  sendBuffer[1 + 0x5 * 3] = 0b00000011; // 3 decimal
  sendBuffer[2 + 0x5 * 3] = 0b00000001; // 1 deccimal

  sendBuffer[0 + 0x6 * 3] = 0x06;
  sendBuffer[1 + 0x6 * 3] = 0b00000110; // 6 decimal
  sendBuffer[2 + 0x6 * 3] = 0b00000010; // 2 deccimal

  sendBuffer[0 + 0x7 * 3] = 0x07;
  sendBuffer[1 + 0x7 * 3] = 0b00000011; // 3 decimal
  sendBuffer[2 + 0x7 * 3] = 0b00000001; // 1 deccimal

  sendBuffer[0 + 0x8 * 3] = 0x08;
  sendBuffer[1 + 0x8 * 3] = 0b00000110; // 6 decimal
  sendBuffer[2 + 0x8 * 3] = 0b00000010; // 2 deccimal

  sendBuffer[0 + 0x9 * 3] = 0x09;
  sendBuffer[1 + 0x9 * 3] = 0x31; // 9 decimal
  sendBuffer[2 + 0x9 * 3] = 0b00000011; // 3 deccimal

  sendBuffer[0 + 0xA * 3] = 0x0A;
  sendBuffer[1 + 0xA * 3] = 0x00;
  sendBuffer[2 + 0xA * 3] = 0x00; // currently not breaking!
  
  sendBuffer[0 + 0xB * 3] = 0x0B;
  sendBuffer[1 + 0xB * 3] = 0x00;
  sendBuffer[2 + 0xB * 3] = 0x01; // Levitating (feat. DaBaby) - Dua Lipa
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

// if there is something coming from raspberry, executes it
// else nothing happens.
void listenRaspberry() {
  if (readSerial()) {
    messageEncoder(); // Runs the command that raspbery sent.
  }
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
bool messageEncoder() {
  if (receiveBuffer[0] == 0x01) {
    uint16_t newPWM = (receiveBuffer[1] << 8) | receiveBuffer[2];
    setInductionMotorPWM(newPWM);
  } else if (receiveBuffer[0] == 0x0A) {
    setBrakes((receiveBuffer[2] & 0x01) == 1);
  } else if (receiveBuffer[0] == 0x0B) {
    setLevitation((receiveBuffer[2] & 0x01) == 1);
  } else {
    return false;
  }

  return true;
}

void setInductionMotorPWM(uint16_t newPWM) {
  sendBuffer[0 + 0x1 * 3] = 0x01;
  sendBuffer[1 + 0x1 * 3] = 0x00;
  sendBuffer[2 + 0x1 * 3] = 0x0C;
}

void setBrakes(bool newBrakeStatus) {
  sendBuffer[0 + 0xA * 3] = 0x0A;
  sendBuffer[1 + 0xA * 3] = 0x00;
  sendBuffer[2 + 0xA * 3] = 0x01; // BREAKING!
}

void setLevitation(bool newLevitationStatus) {
  sendBuffer[0 + 0xB * 3] = 0x0B;
  sendBuffer[1 + 0xB * 3] = 0x00;
  sendBuffer[2 + 0xB * 3] = 0x00; // stop levitation
}

