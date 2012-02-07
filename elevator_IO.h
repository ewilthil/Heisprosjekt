#ifndef MAIN_H
#define MAIN_H

#include "elev.h"

#define elev_button_type_t button_type_t

void io_resetStopLight();
void io_resetDoorLight();
void io_resetAllLights();
void io_resetAllFloorLights();
void io_resetFloorLightsOnTemporaryStop(enum floor_t, enum direction_t);

void io_setStopLight();
void io_setDoorLight();
void io_setFloorLight(enum floor_t);
void io_setFloorLightsOnOrder(enum button_type_t,enum floor_t);

void io_startMotor(enum direction_t);
void io_stopMotor();



#endif
