#include <Arduino.h>

const int BUFFERSIZE = 3; // Buffer size for 2 bytes

byte sendBuffer[bufferSize]; // Buffer for sending data to Raspberry Pi
byte receiveBuffer[bufferSize]; // Buffer for receiving data from Raspberry Pi

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Prepare data to send (replace with your actual logic)
  sendBuffer[0] = 0x03; // Example data byte 1
  sendBuffer[1] = 0x01; // Example data byte 2

  // Send data to Raspberry Pi
  Serial.write(dataBuffer, bufferSize);

  if (Serial.available() >= bufferSize) {
      Serial.readBytes(receiveBuffer, bufferSize);
  }

  sendBuffer[0] = receiveBuffer[0]; // Example data byte 1
  sendBuffer[1] = receiveBuffer[1]; // Example data byte 2

  delay(1000);
}

// message 		byte1(op)   byte2   byte3
//
// Gets the current value of the location sensor or
// pwm of the induction motor. Byte2 and byte3
// combined is the uint16 value.
// location 	  0x00         uint16
// induction  	  0x01         uint16
//
// Gets the current value the sensors reads.
// byte2 is before dot(.) and byte 3 is after dot(.)
// integer part can have a value between 0-2^8
// fractional part must be less than 100
// temp1 	  	  0x02      uint8   uint8
// temp2 	  	  0x03      uint8   uint8
// temp3 	  	  0x04      uint8   uint8
// current 	  	  0x05      uint8   uint8
// voltage 	  	  0x06      uint8   uint8
// gyro_x 	  	  0x07      uint8   uint8
// gyro_y 	  	  0x08      uint8   uint8
// gyro_z 	  	  0x09      uint8   uint8
//
// Gets the current status depending on the last bit.
// if the last bit is 0 brake/levitation is off
// if the last bit is 1 brake/levitation is on
// brake 	  	  0x0A      0x00 	uint8
// levitation 	  0x0B      0x00 	uint8
void messageEncoder(const uint8_t messageBytes[], size_t messageLength) {
  uint16_t valueUi16;
  float valueFloat32;
  bool valueStatus;

  switch (messageBytes[0]) {
    case 0x01:
      valueUi16 = (messageBytes[1] << 8) | messageBytes[2];
      std::cout << "Induction motor PWM: " << valueUi16 << std::endl;
                  break;
    case 0x0A:
    case 0x0B:
      valueStatus = (messageBytes[2] & 0x01) == 1;
      std::cout << (messageBytes[0] == 0x0A ? "Brake status: " : "Levitation status: ") << valueStatus << std::endl;
      break;
    default:
      std::cout << "Unknown message type: 0x" << std::hex << messageBytes[0] << std::endl;
  }
}