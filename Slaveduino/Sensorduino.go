package Slaveduino

import (
	"gercek_patron/Serial"
)

const sensorduinoMessageItemCount = 9

type Sensorduino struct {
	PortHandler Serial.Port
	PortName    string
}

func (sensorduino *Sensorduino) GetSensorData() bool {
	for i := 0; i < sensorduinoMessageItemCount; i++ {
		sensorduino.PortHandler.ReadSerialConnectionWithDelay()
		// if the opcode is 0xE that means arduino successfully finished its job.
		if sensorduino.PortHandler.GetReadBuffer()[0] != 0xE {
			MessageEncoder(sensorduino.PortHandler.GetReadBuffer())
		} else {
			return false
		}
	}
	return true
}
