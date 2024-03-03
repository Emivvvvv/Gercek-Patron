package Slaveduino

import (
	"gercek_patron/Serial"
)

const movementduinoMessageItemCount = 3

type Movementduino struct {
	PortHandler Serial.Port
}

func (movementduino *Movementduino) GetMovementData() {
	for i := 0; i < movementduinoMessageItemCount; i++ {
		movementduino.PortHandler.ReadSerialConnection()
		MessageEncoder(movementduino.PortHandler.GetReadBuffer())
	}
}

func (movementduino *Movementduino) SetInductionPWM(newPWM uint16) {
	var byte1 = uint8(newPWM >> 8)
	var byte2 = uint8(newPWM & 0x00FF)

	movementduino.PortHandler.SetWriteBuffer(0x1, byte1, byte2)
	movementduino.PortHandler.SendSerialConnection()
}

func (movementduino *Movementduino) SetBrakeStatus(newStatus bool) {
	var byte2 byte

	switch newStatus {
	case true:
		byte2 = 0x1
	case false:
		byte2 = 0x0
	}

	movementduino.PortHandler.SetWriteBuffer(0xA, 0x0, byte2)
	movementduino.PortHandler.SendSerialConnection()
}

func (movementduino *Movementduino) SetLevitationStatus(newStatus bool) {
	var byte2 byte

	switch newStatus {
	case true:
		byte2 = 0x1
	case false:
		byte2 = 0x0
	}
	movementduino.PortHandler.SetWriteBuffer(0xB, 0x0, byte2)
	movementduino.PortHandler.SendSerialConnection()
}
