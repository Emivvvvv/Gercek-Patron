#include <Arduino.h>

const int BUFFERSIZE = 2;

byte sendBuffer[BUFFERSIZE]; // Buffer for sending data to Raspberry Pi
byte receiveBuffer[BUFFERSIZE]; // Buffer for receiving data from Raspberry Pi

void testConnection();

void setup() {
  Serial.begin(9600); // Must match with Raspberry Pi

}

void loop() {
  void testConnection(); // Run test function.
  delay(1000);
}

// Sends a test message and reads the response from the gercek patron.
void testConnection() {
  // Dummy message to send for testing
  sendBuffer[0] = 0x06;
  sendBuffer[1] = 0x09;

  // Send data to Raspberry Pi
  Serial.write(sendBuffer, BUFFERSIZE);

  // Check if any data is available from Raspberry Pi
  if (Serial.available() >= BUFFERSIZE) {
    Serial.readBytes(receiveBuffer, BUFFERSIZE);
    Serial.write(receiveBuffer, BUFFERSIZE);
  }
}
