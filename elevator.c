#include "elev.h"
#include "elevator.h"

static enum floor_t floor;
static enum direction_t direction;
static int destinationMatrix[NUMBEROFBUTTONTYPES][NUMBEROFFLOORS]={
                      /*1	2	3	4*/
/*CALL_UP*/{		0,	0,	0,	0},
/*CALL_DOWN*/{		0,	0,	0,	0},
/*COMMAND*/{		0,	0,	0,	0}
};


void checkSensor(){
	if(inFloor()){
		handleEvent(FLOOR_REACHED);
	}
}	
/*
floorHasOrder()
og
noObstruction()
er guards for FSM
*/
int floorHasOrder(){
	return (destinationMatrix[direction][floor]||destionationMatrix[COMMAND][floor]);
}
int noObstruction(){
	return !(elev_get_obstruction_signal());
}
/*
addOrderToList er en del av elevator-klassen
*/ 
void addOrderToList(enum elev_button_type_t button,enum floor_t floor){
	destinationMatrix[button][floor]=1;
}
/*
handleStop()
handleEmergencyStop()
handleDestination()
kalles av tilstandsmaskinen ved hhv ankomst etasje, nødstopp og avgang etasje
*/
void handleStop(){
	elev_set_door_open_lamp(1);
	sleep(3);
	if(noObstruction)
		elev_set_door_open_lamp(0);
	else
}
void handleDestination(){
	if(/*heisen har ordre lengre opp*/)
		elev_set_speed(300*dir);
}
