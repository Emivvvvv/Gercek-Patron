package main

import (
	"fmt"
	"go.bug.st/serial"
	"log"
	"time"
)

const bufferSize = 3

type serialPort struct {
	port        serial.Port
	readBuffer  []byte
	writeBuffer []byte
}

func initSerialPort(portName string) *serialPort {
	port, err := serial.Open(portName, &serial.Mode{BaudRate: 115200})
	if err != nil {
		log.Fatal(err)
	}

	readBuffer := make([]byte, bufferSize)  // Buffer for received data
	writeBuffer := make([]byte, bufferSize) // Buffer for data will be sent

	return &serialPort{
		port,
		readBuffer,
		writeBuffer,
	}
}

func (portHandler *serialPort) readFromSerialConnection() {
	n, err := portHandler.port.Read(portHandler.readBuffer)
	if err != nil {
		log.Fatal(err)
	}
	if n == bufferSize { // Check if 2 bytes were received
		fmt.Printf("Received from Arduino: %02X %02X %02X\n", portHandler.readBuffer[0], portHandler.readBuffer[1], portHandler.readBuffer[2])
	} else {
		fmt.Println("Incomplete data received!")
	}
}

func (portHandler *serialPort) sendSerialConnection() {
	sentBytes, err := portHandler.port.Write(portHandler.writeBuffer)
	if err != nil {
		log.Fatal(err)
	}
	if sentBytes != bufferSize {
		fmt.Println("Incomplete data sent!")
	}
	fmt.Printf("Sent to Arduino: %02X %02X %02X\n", portHandler.writeBuffer[0], portHandler.writeBuffer[1], portHandler.writeBuffer[2])
}

func (portHandler *serialPort) start() byte {
	err := portHandler.port.ResetInputBuffer()
	if err != nil {
		log.Fatal("Couldn't start the connection.")
	}
	portHandler.writeBuffer[0] = 0xF
	portHandler.writeBuffer[1] = 0xF
	portHandler.writeBuffer[2] = 0xF
	portHandler.sendSerialConnection()

	portHandler.readFromSerialConnection()
	return portHandler.readBuffer[0]
}

func (portHandler *serialPort) closePort() {
	err := portHandler.port.Close()
	if err != nil {
		return
	}
}

func (portHandler *serialPort) test(arduinoInfo byte) {
	switch arduinoInfo {
	case 0x0C:
		portHandler.testSensor()
	case 0x0D:
		portHandler.testMovement()
	}
}

func (portHandler *serialPort) testSensor() {
	time.Sleep(50 * time.Millisecond)

	portHandler.getSensorData()
	portHandler.closePort()
}

func (portHandler *serialPort) testMovement() {
	time.Sleep(50 * time.Millisecond)

	portHandler.getMovementData()
	portHandler.setLevitationStatus(false)
	portHandler.setBrakeStatus(true)
	portHandler.setInductionPWM(3131)
	portHandler.getMovementData()
	portHandler.closePort()
}

func main() {
	ports, err := serial.GetPortsList()
	if err != nil {
		log.Fatal(err)
	}
	if len(ports) == 0 {
		log.Fatal("No serial ports found!")
	}

	for _, portName := range ports {
		portHandler := initSerialPort(portName)
		arduinoInfo := portHandler.start()
		portHandler.test(arduinoInfo)
		portHandler.closePort()
	}

	fmt.Println("All serial port tests completed.")
}

// There will be an arduino struct. This will be connected to arduino struct not serialPort!
func (portHandler *serialPort) setInductionPWM(newPWM uint16) {
	portHandler.writeBuffer[0] = 0x1

	var byte1 = uint8(newPWM >> 8)
	var byte2 = uint8(newPWM & 0x00FF)

	portHandler.writeBuffer[1] = byte1
	portHandler.writeBuffer[2] = byte2

	portHandler.sendSerialConnection()
}

// There will be an arduino struct. This will be connected to arduino struct not serialPort!
func (portHandler *serialPort) setBrakeStatus(newStatus bool) {
	portHandler.writeBuffer[0] = 0xA
	portHandler.writeBuffer[1] = 0x0

	switch newStatus {
	case true:
		portHandler.writeBuffer[2] = 0x1
	case false:
		portHandler.writeBuffer[2] = 0x0
	}

	portHandler.sendSerialConnection()
}

// There will be an arduino struct. This will be connected to arduino struct not serialPort!
func (portHandler *serialPort) setLevitationStatus(newStatus bool) {
	portHandler.writeBuffer[0] = 0xB
	portHandler.writeBuffer[1] = 0x0

	switch newStatus {
	case true:
		portHandler.writeBuffer[2] = 0x1
	case false:
		portHandler.writeBuffer[2] = 0x0
	}

	portHandler.writeBuffer[2] = 0x0

	portHandler.sendSerialConnection()
}

// There will be a sensor arduino struct. This will be connected to sensor arduino struct not serialPort!
func (portHandler *serialPort) getSensorData() {
	for i := 0; i < 9; i++ {
		portHandler.readFromSerialConnection()
		messageEncoder(portHandler.readBuffer)
		time.Sleep(2 * time.Millisecond)
	}
}

// There will be a movement arduino struct. This will be connected to movement arduino struct not serialPort!
func (portHandler *serialPort) getMovementData() {
	for i := 0; i < 3; i++ {
		portHandler.readFromSerialConnection()
		messageEncoder(portHandler.readBuffer)
		time.Sleep(2 * time.Millisecond)
	}
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
func messageEncoder(messageBytes []byte) {
	var valueUi16 uint16
	var valueFloat32 float32
	var valueStatus bool

	switch messageBytes[0] {
	case 0x00, 0x01:
		valueUi16 = (uint16(messageBytes[1]) << 8) + uint16(messageBytes[2])
	case 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09:
		valueFloat32 = float32(messageBytes[1])
		if messageBytes[2] < 0x9 {
			valueFloat32 += float32(messageBytes[2]) / 10
		} else {
			valueFloat32 += float32(messageBytes[2]) / 100
		}
	case 0x0A, 0x0B:
		valueStatus = messageBytes[2]&0x01 == 1
	}

	switch messageBytes[0] {
	case 0x00:
		fmt.Println("Location: ", valueUi16)
	case 0x01:
		fmt.Println("Induction motor PWM value: ", valueUi16)
	case 0x02:
		fmt.Println("Temp 1: ", valueFloat32)
	case 0x03:
		fmt.Println("Temp 2: ", valueFloat32)
	case 0x04:
		fmt.Println("Temp 3: ", valueFloat32)
	case 0x05:
		fmt.Println("Current: ", valueFloat32)
	case 0x06:
		fmt.Println("Voltage: ", valueFloat32)
	case 0x07:
		fmt.Println("Gyro x: ", valueFloat32)
	case 0x08:
		fmt.Println("Gyro y: ", valueFloat32)
	case 0x09:
		fmt.Println("Gyro z: ", valueFloat32)
	case 0x0A:
		fmt.Println("Brake status: ", valueStatus)
	case 0x0B:
		fmt.Println("Levitation status: ", valueStatus)
	default:
		fmt.Println("The duck is this message bro! LOLLL: ", messageBytes)
	}
}
