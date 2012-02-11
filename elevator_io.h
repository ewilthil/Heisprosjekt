#ifndef MAIN_H
#define MAIN_H

#include "elev.h"

#define elev_button_type_t buttonType_t

void io_resetStopLight();
void io_closeDoor();
void io_resetAllButtonLights();
void io_resetFloorLightsOnTemporaryStop(floor_t, direction_t);
void io_resetButtonLight(buttonType_t,floor_t);
void io_setStopLight();
void io_openDoor();
//void io_setFloorCallLight(floor_t, direction_t);
void io_setButtonLight(buttonType_t, floor_t);
//void io_setCommandLight(floor_t);
void io_setFloorIndicator(floor_t);

void io_startMotor(direction_t);
void io_stopMotor();
int io_elevatorIsObstructed();
int io_elevatorIsAtFloor();


#endif
