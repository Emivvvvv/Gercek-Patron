package Serial

import (
	"fmt"
	"go.bug.st/serial"
	"log"
	"time"
)

const bufferSize = 3

type Port struct {
	port        serial.Port
	readBuffer  []byte
	writeBuffer []byte
}

func GetPortNames() []string {
	ports, err := serial.GetPortsList()
	if err != nil {
		log.Fatal(err)
	}
	if len(ports) == 0 {
		log.Fatal("No serial ports found!")
	}

	return ports
}

func InitSerialPort(portName string) *Port {
	port, err := serial.Open(portName, &serial.Mode{BaudRate: 115200})
	if err != nil {
		log.Fatal(err)
	}

	err = port.ResetOutputBuffer()
	if err != nil {
		log.Fatal(err)
	}
	err = port.ResetInputBuffer()
	if err != nil {
		log.Fatal(err)
	}

	readBuffer := make([]byte, bufferSize)  // Buffer for received data
	writeBuffer := make([]byte, bufferSize) // Buffer for data will be sent

	return &Port{
		port,
		readBuffer,
		writeBuffer,
	}
}

func (portHandler *Port) ReadSerialConnection() {
	n, err := portHandler.port.Read(portHandler.readBuffer)
	if err != nil {
		log.Fatal(err)
	}
	if n == bufferSize { // Check if 2 bytes were received
		//fmt.Printf("Received from Arduino: %02X %02X %02X\n", portHandler.readBuffer[0], portHandler.readBuffer[1], portHandler.readBuffer[2])
	} else {
		fmt.Println("Incomplete data received!")
	}
}

func (portHandler *Port) ReadSerialConnectionWithDelay() {
	time.Sleep(3000 * time.Microsecond)
	portHandler.ReadSerialConnection()
}
func (portHandler *Port) SendSerialConnection() {
	sentBytes, err := portHandler.port.Write(portHandler.writeBuffer)
	if err != nil {
		log.Fatal(err)
	}
	if sentBytes != bufferSize {
		fmt.Println("Incomplete data sent!")
	}
	fmt.Printf("Sent to Arduino: %02X %02X %02X\n", portHandler.writeBuffer[0], portHandler.writeBuffer[1], portHandler.writeBuffer[2])
}

func (portHandler *Port) SetWriteBuffer(byte0, byte1, byte2 byte) {
	portHandler.writeBuffer[0] = byte0
	portHandler.writeBuffer[1] = byte1
	portHandler.writeBuffer[2] = byte2
}

func (portHandler *Port) GetReadBuffer() []byte {
	return portHandler.readBuffer
}

func (portHandler *Port) Start() {
	err := portHandler.port.ResetInputBuffer()
	if err != nil {
		log.Fatal("Couldn't start the connection.")
	}
	portHandler.writeBuffer[0] = 0xF
	portHandler.writeBuffer[1] = 0xF
	portHandler.writeBuffer[2] = 0xF
	portHandler.SendSerialConnection()
}

func (portHandler *Port) ClosePort() {
	err := portHandler.port.Close()
	if err != nil {
		return
	}
}
