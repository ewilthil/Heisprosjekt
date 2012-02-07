#ifndef ELEVATOR_FSM_H
#define ELEVATOR_FSM_H

#define NUMBEROFSTATES 4
#define NUMBEROFEVENTS 3

enum event_t{NEW_DESTINATION, FLOOR_REACHED, STOP_PRESSED};
enum state_t{EXECUTING_ORDER, TEMPORARY_STOP, IDLE, EMERGENCY_STOP};

void handleEvent(enum event_t event);

#endif



