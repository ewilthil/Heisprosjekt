#ifndef ELEVATOR_IO_H
#define ELEVATOR_IO_H


#include <unistd.h>
#include "elev.h"
#include "elevator_ctrl.h"
#include "elevator_timer.h"

#define buttonType_t elev_button_type_t
/*"lysbrytere"*/
void io_setStopLight();
void io_resetStopLight();
void io_setButtonLight(buttonType_t, int);
void io_resetButtonLight(buttonType_t,int);
void io_resetAllButtonLights();
void io_setFloorIndicator(int);
/*heisaksjoner*/
void io_openDoor();
void io_closeDoor();
void io_startMotor();
void io_stopMotor();
void io_stopMotorEmergency();
/*informasjon om heis*/
int io_elevatorIsObstructed();
int io_elevatorIsInFloor();
int io_getCurrentFloor();
int io_motorIsRunning();
/*knapperegistrering*/
int io_emergencyStopPressed();
int io_orderButtonPressed(buttonType_t, int);
/*guard for SM*/
int io_doorClosed();
#endif
