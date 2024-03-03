package main

import (
	"fmt"
	"gercek_patron/Serial"
	"gercek_patron/Slaveduino"
	"log"
	"sync"
	"time"
)

func main() {
	var (
		isSensorDuinoConnected   bool
		isMovementDuinoConnected bool
		sensorDuino              Slaveduino.Sensorduino
		movementDuino            Slaveduino.Movementduino
		wgArduino                sync.WaitGroup
		wgOperations             sync.WaitGroup
		resultChan               = make(chan interface{})
	)

	portNames := Serial.GetPortNames()
	wgArduino.Add(len(portNames))

	for _, portName := range portNames {
		go connectArduino(portName, resultChan, &wgArduino)
	}

	// Wait for all Goroutines to finish and collect results
	go func() {
		wgArduino.Wait()
		close(resultChan)
	}()

	for result := range resultChan {
		switch arduino := result.(type) {
		case Slaveduino.Sensorduino:
			sensorDuino = arduino
			isSensorDuinoConnected = true
			fmt.Println("Sensor Arduino at Port: ", sensorDuino.PortName, " successfully connected!")
		case Slaveduino.Movementduino:
			movementDuino = arduino
			isMovementDuinoConnected = true
			fmt.Println("Movement Arduino at Port: ", movementDuino.PortName, " successfully connected!")
		}
	}

	if isSensorDuinoConnected {
		wgOperations.Add(1)
		go func() {
			defer wgOperations.Done()
			sensorDuino.GetSensorData()
			sensorDuino.GetSensorData()
		}()
	}

	if isMovementDuinoConnected {
		wgOperations.Add(1)
		go func() {
			defer wgOperations.Done()
			movementDuino.GetMovementData()
			movementDuino.SetLevitationStatus(false)
			movementDuino.SetInductionPWM(3169)
			movementDuino.SetBrakeStatus(true)
			time.Sleep(1 * time.Second)
			movementDuino.GetMovementData()
		}()
	}

	wgOperations.Wait()
}

func connectArduino(portName string, resultChan chan<- interface{}, wg *sync.WaitGroup) {
	defer wg.Done()

	portHandler := Serial.InitSerialPort(portName)
	portHandler.Start()
	portHandler.ReadSerialConnectionWithDelay()
	identifier := portHandler.GetReadBuffer()[0]

	var arduino interface{}

	if identifier == 0xC {
		arduino = Slaveduino.Sensorduino{PortHandler: *portHandler, PortName: portName}
	} else if identifier == 0xD {
		arduino = Slaveduino.Movementduino{PortHandler: *portHandler, PortName: portName}
	} else {
		log.Fatal("Couldn't get the Arduino type!")
	}

	resultChan <- arduino
}
