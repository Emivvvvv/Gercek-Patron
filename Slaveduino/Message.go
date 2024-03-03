package Slaveduino

import "fmt"

// MessageEncoder message 		byte1(op)   byte2   byte3
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
func MessageEncoder(messageBytes []byte) {
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
		fmt.Println("The duck is this message bro! LOL: ", messageBytes)
	}
}
