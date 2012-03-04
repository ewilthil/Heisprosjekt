#include "elevator_io.h"

static int motorIsRunning=0;
static int doorClosed=1;
/*kontroll av lys*/
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
	for(floor=0;floor<NUMBER_OF_FLOORS;floor++){
		elev_set_button_lamp(BUTTON_COMMAND,floor,0);
		if(floor!=0)
			elev_set_button_lamp(BUTTON_CALL_DOWN,floor,0);
		if(floor<(NUMBER_OF_FLOORS-1))
			elev_set_button_lamp(BUTTON_CALL_UP,floor,0);
	}
}
void io_setFloorIndicator(int floor){
	elev_set_floor_indicator(floor);
}

/*styring*/
void io_openDoor(){
	timer_startDoorTimer();
	elev_set_door_open_lamp(1);
	doorClosed=0;
}
void io_closeDoor(){
	timer_stopDoorTimer();
	elev_set_door_open_lamp(0);
	doorClosed=1;
}
void io_startMotor(){
	direction_t direction = ctrl_getDirection();
	motorIsRunning=1;
	//Direction er enten -1 eller 1, derfor er det mulig å gi pådrag i ønsket retning ved å gange med retning.
	elev_set_speed(300*direction);
}
void io_stopMotor(){
	direction_t direction=ctrl_getDirection();
	int elevatorSpeed = io_readElevatorSpeed(); 
	if(elevatorSpeed<-200 || elevatorSpeed>200){
		//Direction er enten -1 eller 1, derfor er det mulig å gi pådrag mot retningen ved å gange med et negativt tall
		elev_set_speed(-300*direction);
		//Hvor brått heisen stopper i utgangspunktet varierer veldig mye fra heis til heis og fra gang til gang.
		//Det kan derfor bli nødvendig å forlenge tiden systemet sover, for å få heisen til å bremse som vi ønsker.
		usleep(6000);
	}
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
int io_readElevatorSpeed(){
	int speed=io_read_analog(0);
	//Når heisen er i ro vil io_read_analog(0) returnere ca. 2060, dette er funnet eksperimentelt
	speed -= 2060;
	return speed;
}
int io_doorClosed(){
	return doorClosed;
}

/*knapperegistrering*/
int io_emergencyStopPressed(){
	return elev_get_stop_signal();
}
int io_orderButtonPressed(buttonType_t buttonType, int floor){
	return elev_get_button_signal(buttonType,floor);
}
