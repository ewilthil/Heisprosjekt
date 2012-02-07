#ifndef MAIN_H
#define MAIN_H

#include "elev.h"

void resetStopLight();
void resetDoorLight();
void resetAllLights();
void resetAllFloorLights();
void resetFloorLightOnTemporaryStop(enum floor_t, enum direction_t);

void startMotor(enum direction_t);
void stopMotor();



#endif
