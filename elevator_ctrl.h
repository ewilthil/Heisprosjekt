#ifndef ELEVATOR_CTRL_H
#define ELEVATOR_CTRL_H

//TODO: stdio.h er kun for feilsøking, kan fjernes i endelig versjon

#include <stdio.h>
#include <time.h>
#include "elevator_sm.h"
#include "elevator_ui.h"
#include "elevator_io.h"

#define NUMBER_OF_FLOORS 4 
#define NUMBER_OF_BUTTON_TYPES 3
#define TOP_FLOOR 3
#define BOTTOM_FLOOR 0

typedef enum {
	UP=1, 
	DOWN=-1
}direction_t;

/*Hjelpefunksjoner, skal bort i endelig versjon*/
void potet(char[]);
void debug_printDestinationMatrix();
/*Oppstartsfunksjon*/
void ctrl_initiateElevator();
/*get*/
direction_t ctrl_getDirection();
int ctrl_elevatorHasBeenObstructed();
/*Actions*/
void ctrl_handleStop();
void ctrl_handleEmergencyStop();
void ctrl_handleDestination();
void ctrl_handleDestinationFromIdle();
void ctrl_handleDestinationFromEM();
void ctrl_handleNewOrder();
/*Guards*/
int ctrl_newOrderFromCommandButton();
int ctrl_newOrderNotInCurrentFloor();
int ctrl_noObstruction();
int ctrl_stopElevatorAtCurrentFloor();
/*Lyttefunksjon*/
void ctrl_checkSensor();
/*Heisstyring*/
void ctrl_setNewDirection();
void ctrl_clearAllOrders();
void ctrl_setLightsAtElevatorStop();
void ctrl_removeOrdersFromCurrentFloor();
void ctrl_removeSingleOrder(buttonType_t);
/*Sammenligning*/
int ctrl_orderAtCurrentFloor();
int ctrl_lowerFloorsHaveOrders();
int ctrl_upperFloorsHaveOrders();
int ctrl_orderListHaveOrders();

#endif //ELEVATOR_CTRL_H
