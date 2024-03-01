#include <Arduino.h>

const byte bufferSize = 2; // Buffer size for 2 bytes

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