#ifndef MAIN_H
#define MAIN_H

#include "elev.h"

#define elev_button_type_t button_type_t

void io_resetStopLight();
void io_closeDoor();
void io_resetAllLights();
void io_resetAllButtonLights();
void io_resetFloorLightsOnTemporaryStop(enum floor_t, enum direction_t);

void io_setStopLight();
void io_openDoor();
void io_setFloorCallLight(enum floor_t, enum direction_t);
void io_setCommandLight(enum floor_t);


void io_startMotor(enum direction_t);
void io_stopMotor();



#endif
