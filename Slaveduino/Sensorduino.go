package Slaveduino

import (
	"gercek_patron/Serial"
)

const sensorduinoMessageItemCount = 9

type Sensorduino struct {
	PortHandler Serial.Port
	PortName    string
}

func (sensorduino *Sensorduino) GetSensorData() {
	for i := 0; i < sensorduinoMessageItemCount; i++ {
		sensorduino.PortHandler.ReadSerialConnectionWithDelay()
		MessageEncoder(sensorduino.PortHandler.GetReadBuffer())
	}
}
