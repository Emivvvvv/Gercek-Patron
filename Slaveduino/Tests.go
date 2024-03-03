package Slaveduino

import (
	"fmt"
	"log"
)

var sensorOddEven = false
var movementOddEven = false

func (sensorduino *Sensorduino) Test() bool {
	sensorOddEven = !sensorOddEven
	for i := 0; i < sensorduinoMessageItemCount; i++ {
		sensorduino.PortHandler.ReadSerialConnectionWithDelay()
		// if the opcode is 0xE that means arduino successfully finished its job.
		if sensorduino.PortHandler.GetReadBuffer()[0] != 0xE {
			sensorTestEncoder(sensorduino.PortHandler.GetReadBuffer())
		} else {
			return false
		}
	}
	return true
}

func (movementduino *Movementduino) Test() bool {
	movementOddEven = !movementOddEven
	for i := 0; i < movementduinoMessageItemCount; i++ {
		movementduino.PortHandler.ReadSerialConnectionWithDelay()
		// if the opcode is 0xE that means arduino successfully finished its job.
		if movementduino.PortHandler.GetReadBuffer()[0] != 0xE {
			movementduino.movementTestEncoder()
		} else {
			return false
		}
	}
	return true
}

func sensorTestEncoder(messageBytes []byte) {
	switch messageBytes[0] {
	case 0x00:
		shouldMatch(messageBytes, []byte{0x00, 0x03, 0x01}, []byte{0x00, 0x06, 0x02}, sensorOddEven)
	case 0x02:
		shouldMatch(messageBytes, []byte{0x02, 0x03, 0x01}, []byte{0x02, 0x03, 0x01}, sensorOddEven)
	case 0x03:
		shouldMatch(messageBytes, []byte{0x03, 0x06, 0x02}, []byte{0x03, 0x06, 0x02}, sensorOddEven)
	case 0x04:
		shouldMatch(messageBytes, []byte{0x04, 0x09, 0x03}, []byte{0x04, 0x03, 0x01}, sensorOddEven)
	case 0x05:
		shouldMatch(messageBytes, []byte{0x05, 0x03, 0x01}, []byte{0x05, 0x03, 0x01}, sensorOddEven)
	case 0x06:
		shouldMatch(messageBytes, []byte{0x06, 0x06, 0x02}, []byte{0x06, 0x06, 0x02}, sensorOddEven)
	case 0x07:
		shouldMatch(messageBytes, []byte{0x07, 0x03, 0x01}, []byte{0x07, 0x03, 0x01}, sensorOddEven)
	case 0x08:
		shouldMatch(messageBytes, []byte{0x08, 0x06, 0x02}, []byte{0x08, 0x06, 0x02}, sensorOddEven)
	case 0x09:
		shouldMatch(messageBytes, []byte{0x09, 0x09, 0x03}, []byte{0x09, 0x03, 0x01}, sensorOddEven)
	default:
		log.Fatal("Get rekt noob sensor arduino kaboom")
	}
}

func (movementduino *Movementduino) movementTestEncoder() {
	messageBytes := movementduino.PortHandler.GetReadBuffer()
	switch messageBytes[0] {
	case 0x01:
		shouldMatch(messageBytes, []byte{0x01, 0x00, 0x31}, []byte{0x01, 0x00, 0x62}, movementOddEven)
		if movementOddEven {
			movementduino.SetInductionPWM(98)
		} else {
			movementduino.SetInductionPWM(49)
		}
	case 0x0A:
		shouldMatch(messageBytes, []byte{0x0A, 0x00, 0x00}, []byte{0x0A, 0x00, 0x01}, movementOddEven)
		if movementOddEven {
			movementduino.SetBrakeStatus(true)
		} else {
			movementduino.SetBrakeStatus(false)
		}
	case 0x0B:
		shouldMatch(messageBytes, []byte{0x0B, 0x00, 0x01}, []byte{0x0B, 0x00, 0x00}, movementOddEven)
		if movementOddEven {
			movementduino.SetLevitationStatus(false)
		} else {
			movementduino.SetLevitationStatus(true)
		}
	default:
		log.Fatal("Get rekt movement arduino kaboom")
	}
}

func shouldMatch(messageBytes []byte, expectedBytesOdd []byte, expectedBytesEven []byte, oddEven bool) {
	if oddEven {
		if messageBytes[0] != expectedBytesOdd[0] || messageBytes[1] != expectedBytesOdd[1] || messageBytes[2] != expectedBytesOdd[2] {
			fmt.Println(messageBytes, expectedBytesOdd, expectedBytesEven, oddEven)
			fmt.Println("lol message incorrect haha that means 10 hours of debugging for you!")
		}
	} else {
		if messageBytes[0] != expectedBytesEven[0] || messageBytes[1] != expectedBytesEven[1] || messageBytes[2] != expectedBytesEven[2] {
			fmt.Println(messageBytes, expectedBytesOdd, expectedBytesEven, oddEven)
			fmt.Println("lol message incorrect haha that means 10 hours of debugging for you!")
		}
	}
}
