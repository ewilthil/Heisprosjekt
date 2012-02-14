#ifndef ELEVATOR_CTRL_H
#define ELEVATOR_CTRL_H

#include "elev.h"
#include <time.h>


#define NUMBEROFFLOORS 4 
#define NUMBEROFBUTTONTYPES 3
#define COMMAND 2


typedef enum {
	UP=1, 
	DOWN=-1
}direction_t;
/*Skal slettes ved første anledning
typedef enum {
	FIRST=0,
	SECOND=1,
	THIRD=2,
	FOURTH=3
}floor_t;
*/


//direction_t direction = UP;

void debug_printDestinationMatrix();
/*hovedfunksjoner*/
void ctrl_initiateElevator();
int ctrl_elevatorObstructed();
int ctrl_floorHasOrder();
void ctrl_handleStop();
void ctrl_handleEmergencyStop();
void ctrl_handleDestination();
void ctrl_addOrderToList( elev_button_type_t,  int);
void ctrl_checkSensor();
/*Støttefunksjoner*/
void ctrl_setNewDirection();
int ctrl_checkForOrdersInCurrentDirection();
int ctrl_checkForOrdersInOppositeDirection();
int ctrl_checkLowerFloorsForOrders();
int ctrl_checkUpperFloorsForOrders();
void ctrl_clearDestinationMatrix();
void ctrl_setLightsAtElevatorStop();




#endif //ELEVATOR_CTRL_H
