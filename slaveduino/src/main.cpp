#include <Arduino.h>

const int BUFFERSIZE = 3; // Buffer array size. 3 bytes for each message.

byte sendBuffer[BUFFERSIZE]; // Buffer for sending data to Raspberry Pi
byte receiveBuffer[BUFFERSIZE]; // Buffer for receiving data from Raspberry Pi

uint16_t valueUi16;
float valueFloat32;
bool valueStatus;

void sendLocation();
void   sendInductionPWM();
void   sendTemp1();
void   gercekPatronIsi();
void   sendTemp2();
void   sendTemp3();
void   gercekPatronIsi();
void   sendCurrent();
void   sendVoltage();
void   gercekPatronIsi();
void   sendGyroX();
void   sendGyroY();
void   gercekPatronIsi();
void   sendGyroZ();
void   sendBrake();
void   gercekPatronIsi();
void   sendLevitation();
void   gercekPatronIsi();
void sendSerial();
bool readSerial();
void setInductionMotorPWM(uint16_t);
void setBrakes(bool);
void setLevitation(bool);
bool messageEncoder()


void setup() {
  Serial.begin(9600);
  delay(2000);
}

void loop() {
  sendLocation();
  delay(250);
  sendInductionPWM();
  delay(250);
  sendTemp1();
  delay(250);
  gercekPatronIsi();
  delay(250);
  sendTemp2();
  delay(250);
  sendTemp3();
  delay(250);
  gercekPatronIsi();
  delay(250);
  sendCurrent();
  delay(250);
  sendVoltage();
  delay(250);
  gercekPatronIsi();
  delay(250);
  sendGyroX();
  delay(250);
  sendGyroY();
  delay(250);
  gercekPatronIsi();
  delay(250);
  sendGyroZ();
  delay(250);
  sendBrake();
  delay(250);
  gercekPatronIsi();
  delay(250);
  sendLevitation();
  delay(250);
  gercekPatronIsi();
  delay(250);
}

void sendLocation() {
  // dummy message for testing
  sendBuffer[0] = 0x00;
  sendBuffer[1] = 0x03;
  sendBuffer[2] = 0x01;
  sendSerial();
}

void sendInductionPWM() {
  // dummy message for testing
  sendBuffer[0] = 0x01;
  sendBuffer[1] = 0x03;
  sendBuffer[2] = 0x01;
  sendSerial();
}

void sendTemp1() {
  // dummy message for testing
  sendBuffer[0] = 0x02;
  sendBuffer[1] = 0b00000011; // 3 decimal
  sendBuffer[2] = 0b00000001; // 1 deccimal
  sendSerial(); // should be 3.1
}

void sendTemp2() {
  // dummy message for testing
  sendBuffer[0] = 0x03;
  sendBuffer[1] = 0b00000110; // 6 decimal
  sendBuffer[2] = 0b00000010; // 2 deccimal
  sendSerial(); // should be 6.2
}

void sendTemp3() {
  // dummy message for testing
  sendBuffer[0] = 0x04;
  sendBuffer[1] = 0b00001001; // 9 decimal
  sendBuffer[2] = 0b00000011; // 3 deccimal
  sendSerial(); // should be 9.3
}

void sendCurrent() {
  // dummy message for testing
  sendBuffer[0] = 0x05;
  sendBuffer[1] = 0b00000011; // 3 decimal
  sendBuffer[2] = 0b00000001; // 1 deccimal
  sendSerial(); // should be 3.1
}

void sendVoltage() {
  // dummy message for testing
  sendBuffer[0] = 0x06;
  sendBuffer[1] = 0b00000110; // 6 decimal
  sendBuffer[2] = 0b00000010; // 2 deccimal
  sendSerial(); // should be 6.2
}

void sendGyroX() {
  // dummy message for testing
  sendBuffer[0] = 0x07;
  sendBuffer[1] = 0b00000011; // 3 decimal
  sendBuffer[2] = 0b00000001; // 1 deccimal
  sendSerial(); // should be 3.1
}

void sendGyroY() {
  // dummy message for testing
  sendBuffer[0] = 0x08;
  sendBuffer[1] = 0b00000110; // 6 decimal
  sendBuffer[2] = 0b00000010; // 2 deccimal
  sendSerial(); // should be 6.2
}

void sendGyroZ() {
  // dummy message for testing
  sendBuffer[0] = 0x09;
  sendBuffer[1] = 0b00001001; // 9 decimal
  sendBuffer[2] = 0b00000011; // 3 deccimal
  sendSerial(); // should be 9.3
}

void sendBrake() {
  // dummy message for testing
  sendBuffer[0] = 0x0A;
  sendBuffer[1] = 0x00;
  sendBuffer[2] = 0x00; // currently not breaking!
  sendSerial();
}

void sendLevitation() {
  // dummy message for testing
  sendBuffer[0] = 0x0B;
  sendBuffer[1] = 0x00;
  sendBuffer[2] = 0x01; // Levitating (feat. DaBaby) - Dua Lipa
  sendSerial();
}

// sends the sendBuffer to Raspberry Pi
void sendSerial() {
  Serial.write(sendBuffer, BUFFERSIZE);
}

// returns true if the receiveBuffer is changed after call,
// returns false if the receiveBuffer is not changed.
bool readSerial() {
  if (Serial.available() >= BUFFERSIZE) {
      Serial.readBytes(receiveBuffer, BUFFERSIZE);
  }
}

// if there is something coming from raspberry, executes it
// else nothing happens.
void gercekPatronIsi() {
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
  switch (receiveBuffer[0]) {
    case 0x01:
      uint16_t newPWM = (receiveBuffer[1] << 8) | receiveBuffer[2];
      setInductionMotorPWM(newPWM);
    case 0x0A:
    case 0x0B:
      bool newStatus = (receiveBuffer[2] & 0x01) == 1;
      receiveBuffer[0] == 0x0A ? setBrakes(newStatus) : setLevitation(newStatus);
      break;
    default:
      return false;
  }

  return true;
}

void setInductionMotorPWM(uint16_t newPWM) {
  // dummy message for testing (Op code 0x0C)
  sendBuffer[0] = 0x0C;
  sendBuffer[1] = 0x00;
  sendBuffer[2] = 0x00;
  sendSerial();
}

void setBrakes(bool newBrakeStatus) {
  // dummy message for testing (Op code 0x0D)
  sendBuffer[0] = 0x0D;
  sendBuffer[1] = 0x00;
  sendBuffer[2] = 0x00;
  sendSerial();
}

void setLevitation(bool newLevitationStatus) {
  // dummy message for testing (Op code 0x0E)
  sendBuffer[0] = 0x0E;
  sendBuffer[1] = 0x00;
  sendBuffer[2] = 0x00;
  sendSerial();
}

