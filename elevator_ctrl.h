/*
Spørsmål eller kommentarer?
*/
#ifndef MAIN_H
#define MAIN_H

#include <time.h>

#define NUMBEROFFLOORS 4 
#define NUMBEROFBUTTONTYPES 3
#define COMMAND 2

enum direction_t{UP=1, DOWN=-1};
enum floor_t{FIRST=0,SECOND=1,THIRD=2,FOURTH=3};


/*hovedfunksjoner*/
int ctrl_elevatorObstructed();
int ctrl_floorHasOrder();
void ctrl_handleStop();
void ctrl_handleEmergencyStop();
void ctrl_handleDestination();
void ctrl_addOrderToList(buttonType,floor);
void ctrl_checkSensor();
/*Støttefunksjoner*/
int ctrl_checkOrderInThisDirection();
int ctrl_checkOrderInOtherDirection();
int ctrl_checkLowerFloorsForOrders();
int ctrl_checkUpperFloorsForOrders();
void ctrl_clearDestinationMatrix();




#endif //MAIN_H
