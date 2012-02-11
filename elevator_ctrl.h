/*
Spørsmål eller kommentarer?
*/
#ifndef MAIN_H
#define MAIN_H

#include "elev.h"
#include <time.h>

#define NUMBEROFFLOORS 4 
#define NUMBEROFBUTTONTYPES 3
#define COMMAND 2

typedef enum {
	UP=1, 
	DOWN=-1
}direction_t;

typedef enum {
	FIRST=0,
	SECOND=1,
	THIRD=2,
	FOURTH=3
}floor_t;


/*hovedfunksjoner*/
void ctrl_initiateElevator();
int ctrl_elevatorObstructed();
int ctrl_floorHasOrder();
void ctrl_handleStop();
void ctrl_handleEmergencyStop();
void ctrl_handleDestination();
void ctrl_addOrderToList( elev_button_type_t,  floor_t);
void ctrl_checkSensor();
/*Støttefunksjoner*/
int ctrl_checkOrderInThisDirection();
int ctrl_checkOrderInOtherDirection();
int ctrl_checkLowerFloorsForOrders();
int ctrl_checkUpperFloorsForOrders();
void ctrl_clearDestinationMatrix();
void ctrl_setLightsAtElevatorStop();




#endif //MAIN_H
