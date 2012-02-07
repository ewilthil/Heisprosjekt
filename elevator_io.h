#ifndef MAIN_H
#define MAIN_H

#include "elev.h"

#define elev_button_type_t button_type_t

void io_resetStopLight();
void io_closeDoor();
void io_resetAllButtonLights();
void io_resetFloorLightsOnTemporaryStop(floor_t, direction_t);

void io_setStopLight();
void io_openDoor();
void io_setFloorCallLight(floor_t, direction_t);
void io_setCommandLight(floor_t);
void io_setFloorIndicator(floor_t);

void io_startMotor(direction_t);
void io_stopMotor();



#endif
