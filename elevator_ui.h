#ifndef ELEVATOR_UI_H
#define ELEVATOR_UI_H

//#include "elevator_io.h"
#include "elevator_sm.h"
#include "elevator_ctrl.h"
#include "elev.h"

extern int elevatorHasBeenObstructed;

void ui_checkStop();
void ui_checkButtons();
void ui_checkObstruction();

#endif 
