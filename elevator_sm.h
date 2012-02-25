#ifndef ELEVATOR_SM_H
#define ELEVATOR_SM_H

//TODO: stdio.h og printState skal bort i endelig versjon
#include "elevator_ctrl.h"
#include <stdio.h>

#define NUMBEROFSTATES 4
#define NUMBEROFEVENTS 4

typedef enum{
	NEW_DESTINATION, 
	FLOOR_REACHED,
	STOP_PRESSED,
	ORDER_BUTTON_PRESSED
}event_t;
typedef enum{
	EXECUTING_ORDER, 
	TEMPORARY_STOP, 
	IDLE, 
	EMERGENCY_STOP
}state_t;

state_t sm_getState();
void sm_handleEvent(event_t event);
void sm_printState();
#endif



