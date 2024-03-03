package main

import (
	"fmt"
	"gercek_patron/Serial"
	"gercek_patron/Slaveduino"
	"log"
	"time"
)

func main() {
	var sensorDuino Slaveduino.Sensorduino
	var movementDuino Slaveduino.Movementduino
	var isSensorDuinoConnected = false
	var isMovementDuinoConnected = false

	for _, portName := range Serial.GetPortNames() {
		portHandler := Serial.InitSerialPort(portName)
		portHandler.Start()
		portHandler.ReadSerialConnectionWithDelay()
		identifier := portHandler.GetReadBuffer()[0]
		if identifier == 0xC {
			sensorDuino = Slaveduino.Sensorduino{PortHandler: *portHandler}
			isSensorDuinoConnected = true
			fmt.Println("Sensor Arduino at Port: ", portName, " successfully connected!")
		} else if identifier == 0xD {
			movementDuino = Slaveduino.Movementduino{PortHandler: *portHandler}
			isMovementDuinoConnected = true
			fmt.Println("Movement Arduino at Port: ", portName, " successfully connected!")
		} else {
			log.Fatal("Couldn't get the arduino type!")
		}
	}

	if isSensorDuinoConnected {
		sensorDuino.GetSensorData()
		sensorDuino.GetSensorData()
	}

	if isMovementDuinoConnected {
		movementDuino.GetMovementData()
		movementDuino.SetLevitationStatus(false)
		movementDuino.SetInductionPWM(3169)
		movementDuino.SetBrakeStatus(true)
		time.Sleep(1 * time.Second)
		movementDuino.GetMovementData()
	}

	fmt.Println("All serial port tests completed.")
}
