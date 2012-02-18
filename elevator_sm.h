#ifndef ELEVATOR_SM_H
#define ELEVATOR_SM_H

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

void sm_handleEvent(event_t event);
void sm_printState();
#endif



