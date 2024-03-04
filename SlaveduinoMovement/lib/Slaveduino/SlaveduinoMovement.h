#include <Arduino.h>

#ifndef UNTITLED4_SLAVEDUINOMOVEMENT_H
#define UNTITLED4_SLAVEDUINOMOVEMENT_H

const int ITEMCOUNT = 3; // total message that could be sent
const int SENDBUFFERSIZE = ITEMCOUNT * 3; // Buffer array size.
const int RECEIVEBUFFERSIZE = 3; // Buffer array size.

class SlaveduinoMovement {
    private:
        byte sendBuffer[SENDBUFFERSIZE]; // Buffer for sending data to Raspberry Pi
        byte receiveBuffer[RECEIVEBUFFERSIZE]; // Buffer for receiving data from Raspberry Pi
        
        uint16_t valueUi16;
        float valueFloat32;
        bool valueStatus;

        void waitStartSignal();
        void sendArduinoInfo();

        void sendSerial();
        bool readSerial();
        bool messageEncoder();

        void setInductionMotorPWM(uint16_t newPWM);
        void setBrakes(bool newStatus);
        void setLevitation(bool newStatus);

        void getTestData(); // For testing only
        void getTestData2(); // For testing only
    public:
        void init();

        void run();

        void listenRaspberry();

        void setMovementData(uint16_t newPWM, bool newBrakeStatus, bool newLevitationStatus);

        void sendOperationSuccessful();

        void testRun();
};

#endif //UNTITLED4_SLAVEDUINOMOVEMENT_H