#include "elevator_io.h"

int motorIsRunning;
static int doorClosed=1;
/*lyskontrollere*/
void io_setStopLight(){
	elev_set_stop_lamp(1);
}
void io_resetStopLight(){
	elev_set_stop_lamp(0);
}
void io_setButtonLight(buttonType_t button, int floor){
	elev_set_button_lamp(button,floor,1);
}
void io_resetButtonLight(buttonType_t button, int floor){
	elev_set_button_lamp(button,floor,0);
}
void io_resetAllButtonLights(){
	int floor;
	for(floor=0;floor<N_FLOORS;floor++){
		elev_set_button_lamp(BUTTON_COMMAND,floor,0);
		if(floor!=0)
			elev_set_button_lamp(BUTTON_CALL_DOWN,floor,0);
		if(floor<(N_FLOORS-1))
			elev_set_button_lamp(BUTTON_CALL_UP,floor,0);
	}
}
void io_setFloorIndicator(int floor){
	elev_set_floor_indicator(floor);
}

/*heisaksjoner*/
void io_openDoor(){
	elev_set_door_open_lamp(1);
	doorClosed=0;
}
void io_closeDoor(){
	elev_set_door_open_lamp(0);
	doorClosed=1;
}
void io_startMotor(){
	direction_t direction = ctrl_getDirection();
	motorIsRunning=1;
	elev_set_speed(300*direction);
}
void io_stopMotor(){
	direction_t direction=ctrl_getDirection();
	if(motorIsRunning){
		elev_set_speed(-300*direction);
		usleep(6000);
	}
	motorIsRunning=0;
	elev_set_speed(0);
}
void io_stopMotorEmergency(){
	motorIsRunning=0;
	elev_set_speed(0);
}

/*informasjon om heis*/
int io_elevatorIsObstructed(){
	return elev_get_obstruction_signal();
}
int io_elevatorIsInFloor(){
	return elev_get_floor_sensor_signal()+1;
}
int io_getCurrentFloor(){
	return elev_get_floor_sensor_signal();
}
int io_motorIsRunning(){
	return motorIsRunning;
}

/*knapperegistrering*/
int io_EmergencyStopPressed(){
	return elev_get_stop_signal();
}
int io_getButtonPush(buttonType_t buttonType, int floor){
	return elev_get_button_signal(buttonType,floor);
}

/*guard for SM*/
int io_doorClosed(){
	return doorClosed;
}

void io_resetFloorLightsOnTemporaryStop(int floor){
	direction_t direction=ctrl_getDirection();
	elev_set_button_lamp(BUTTON_COMMAND,floor,0);
	if(direction==UP)
		elev_set_button_lamp(BUTTON_CALL_UP,floor,0);
	else if(direction==DOWN)
		elev_set_button_lamp(BUTTON_CALL_DOWN,floor,0);
}

