#include <Arduino.h>

const int BUFFERSIZE = 2;

byte sendBuffer[BUFFERSIZE]; // Buffer for sending data to Raspberry Pi
byte receiveBuffer[BUFFERSIZE]; // Buffer for receiving data from Raspberry Pi

void testConnection();

void setup() {
  Serial.begin(9600); // Must match with Raspberry Pi
  void testConnection(); // Test function. will send
}

void loop() {
  
}

void testConnection() {
  // Dummy message for testing
  sendBuffer[0] = 0x03; 
  sendBuffer[1] = 0x01; 

  // Send data to Raspberry Pi
  Serial.write(sendBuffer, BUFFERSIZE);

  // Check if any data is available from Raspberry Pi
  if (Serial.available() >= BUFFERSIZE) {
    Serial.readBytes(receiveBuffer, BUFFERSIZE);
    Serial.write(sendBuffer, BUFFERSIZE);
  }
}
