#ifndef ELEVATOR_CTRL_H
#define ELEVATOR_CTRL_H

//TODO: stdio.h er kun for feilsøking, kan fjernes i endelig versjon
#include <stdio.h>
#include <time.h>
#include "elevator_sm.h"
#include "elevator_ui.h"
#include "elevator_io.h"

#define NUMBEROFFLOORS 4 
#define NUMBEROFBUTTONTYPES 3
#define COMMAND 2

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
void ctrl_addOrderToList();
/*Guards*/
int ctrl_orderTypeCommand();
int ctrl_orderNotInCurrentFloor();
int ctrl_noObstruction();
int ctrl_floorHasOrder();
int ctrl_doorClosed();
/*Lyttefunksjon*/
void ctrl_checkSensor();
/*Heisstyring*/
void ctrl_setNewDirection();
void ctrl_clearDestinationMatrix();
void ctrl_setLightsAtElevatorStop();
void ctrl_removeOrder();
/*Sammenligning*/
int ctrl_orderAtCurrentFloor();
int ctrl_checkLowerFloorsForOrders();
int ctrl_checkUpperFloorsForOrders();
int ctrl_orderListHasOrders();

#endif //ELEVATOR_CTRL_H
