#ifndef ELEVATOR_TIMER_H
#define ELEVATOR_TIMER_H

#include <time.h>
#include "elevator_ctrl.h"
#include "elevator_io.h"
#include "elevator_sm.h"

void timer_startDoorTimer();
void timer_checkDoorTimer();
void timer_stopDoorTimer();


#endif
