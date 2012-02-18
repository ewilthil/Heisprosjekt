#ifndef ELEVATOR_CTRL_H
#define ELEVATOR_CTRL_H

#include "elev.h"
#include <time.h>
#include "elevator_sm.h"
#include "elevator_ui.h"

#define NUMBEROFFLOORS 4 
#define NUMBEROFBUTTONTYPES 3
#define COMMAND 2


typedef enum {
	UP=1, 
	DOWN=-1
}direction_t;

extern state_t state;
extern elev_button_type_t lastButtonTypeOrder;
extern int lastFloorOrder;

void debug_printDestinationMatrix();
/*hovedfunksjoner*/
void ctrl_initiateElevator();
int ctrl_doorClosed();
int ctrl_floorHasOrder();
void ctrl_handleStop();
void ctrl_handleEmergencyStop();
void ctrl_handleDestination();
void ctrl_goToOrder();
void ctrl_handleDestinationFromIdle();
void ctrl_addOrderToList();
void ctrl_checkSensor();
int ctrl_orderAtCurrentFloor();
/*Støttefunksjoner*/
void ctrl_setNewDirection();
int ctrl_checkForOrdersInCurrentDirection();
int ctrl_checkForOrdersInOppositeDirection();
int ctrl_checkLowerFloorsForOrders();
int ctrl_checkUpperFloorsForOrders();
void ctrl_clearDestinationMatrix();
void ctrl_setLightsAtElevatorStop();
int ctrl_orderListHasOrders();
void ctrl_removeOrder();
int ctrl_noObstruction();
int ctrl_orderTypeCommand();
int ctrl_orderNotInCurrentFloor();

#endif //ELEVATOR_CTRL_H
