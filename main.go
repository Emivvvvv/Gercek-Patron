package main

import (
	"fmt"
	"go.bug.st/serial"
	"log"
	"time"
)

const bufferSize = 2

type serialPort struct {
	port        serial.Port
	readBuffer  []byte
	writeBuffer []byte
}

func initSerialPort(portName string) *serialPort {
	port, err := serial.Open(portName, &serial.Mode{BaudRate: 9600})
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
		fmt.Printf("Received from Arduino: %02X %02X\n", portHandler.readBuffer[0], portHandler.readBuffer[1])
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
	fmt.Printf("Sent to Arduino: %02X %02X\n", portHandler.writeBuffer[0], portHandler.writeBuffer[1])
}

func (portHandler *serialPort) closePort() {
	err := portHandler.port.Close()
	if err != nil {
		return
	}
}

func main() {
	ports, err := serial.GetPortsList()
	if err != nil {
		log.Fatal(err)
	}
	if len(ports) == 0 {
		log.Fatal("No serial ports found!")
	}
	fmt.Println("Found ports:")
	for index, port := range ports {
		fmt.Printf("%d: %v\n", index, port)
	}

	var index int
	fmt.Print("Type the index of serial connection port that you want to connect: ")
	fmt.Scan(&index)
	fmt.Println("Selected port name: ", ports[index])

	portHandler := initSerialPort(ports[index])

	// Dummy data to send
	portHandler.writeBuffer[0] = 0x06
	portHandler.writeBuffer[1] = 0x09

	for {
		portHandler.sendSerialConnection()
		portHandler.readFromSerialConnection()
		time.Sleep(1 * time.Second)
	}

	portHandler.closePort()
}
