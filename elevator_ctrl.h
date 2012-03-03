#ifndef ELEVATOR_CTRL_H
#define ELEVATOR_CTRL_H

#include "elevator_sm.h"
#include "elevator_ui.h"
#include "elevator_io.h"
#include "elevator_timer.h"

#define NUMBER_OF_FLOORS 4 
#define NUMBER_OF_BUTTON_TYPES 3
#define TOP_FLOOR 3
#define BOTTOM_FLOOR 0

typedef enum {
	UP=1, 
	DOWN=-1
}direction_t;

void ctrl_initiateElevator();
direction_t ctrl_getDirection();
void ctrl_checkSensor();
/*Handlinger for tilstandsmaskin*/
void ctrl_handleStop();
void ctrl_handleEmergencyStop();
void ctrl_handleDestination();
void ctrl_handleDestinationFromIdle();
void ctrl_handleDestinationFromEM();
void ctrl_handleNewOrder();
/*Betingelser for tilstandsmaskin*/
int ctrl_newOrderFromCommandButton();
int ctrl_newOrderNotInCurrentFloor();
int ctrl_noObstruction();
int ctrl_stopElevatorAtCurrentFloor();
int ctrl_orderListHaveOrders();

#endif //ELEVATOR_CTRL_H
