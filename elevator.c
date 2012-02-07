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
bool floorHasOrder(){
	return (destinationMatrix[direction][floor]||destionationMatrix[COMMAND][floor]);
}
bool noObstruction(){
	return !(elev_get_obstruction_signal());
} 
void addOrderToList(enum elev_button_type_t button,enum floor_t floor){
	destinationMatrix[button][floor]=1;
}
