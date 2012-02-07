#include "elevator_IO.h"


void io_resetAllButtonLights(){
	int floor;
	for(floor=0;floor<N_FLOORS;floor++){
		elev_set_button_lamp(BUTTON_COMMAND,floor,0)
		if(floor!=0)
			elev_set_button_lamp(BUTTON_CALL_DOWN,floor,0)
		if(floor<(N_FLOORS-1))
			elev_set_button_lamp(BUTTON_CALL_UP,floor,0)
	}
}
void io_resetStopLight(){
	elev_set_stop_lamp(0);
}
void io_resetFloorLightsOnTemporaryStop(floor_t floor, direction_t direction){
	elev_set_button_lamp(BUTTON_COMMAND,floor,0);
	if(direction==UP)
		elev_set_button_lamp(BUTTON_CALL_UP,floor,0);
	else if(direction==DOWN)
		elev_set_button_lamp(BUTTON_CALL_DOWN,floor,0);
}
void io_closeDoor(){
	elev_set_door_open_lamp(0);
}

void io_setStopLight(){
	elev_set_stop_lamp(1);
}
void io_setFloorCallLight(floor_t floor, direction_t direction){
	if(direction==UP)
		elev_set_button_lamp(BUTTON_CALL_UP,floor,1);
	else if(direction==DOWN)
		elev_set_button_lamp(BUTTON_CALL_DOWN,floor,1);
}
void io_setCommandLight(floor_t floor){
	elev_set_button_lamp(BUTTON_COMMAND,floor,1);
}
void io_openDoor(){
	elev_set_door_open_lamp(1);
}
void io_startMotor(direction_t direction){
	elev_set_speed(300*direction);
}
void io_stopMotor(){
	elev_set_speed(0);
}
