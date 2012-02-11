#include "elevator_io.h"
#include "elevator_ctrl.h"
#include "elevator_sm.h"
#include "elevator_ui.h"


static floor_t floor;
static direction_t direction;
static int destinationMatrix[NUMBEROFBUTTONTYPES][NUMBEROFFLOORS]={
                      /*1	2	3	4*/
/*CALL_UP*/{		0,	0,	0,	0},
/*CALL_DOWN*/{		0,	0,	0,	0},
/*COMMAND*/{		0,	0,	0,	0}
};

void ctrl_initiateElevator(){
	if(io_elevatorIsInFloor()){
		floor=io_getCurrentFloor();
		return;
	}
	else{
		direction=UP;
		io_startMotor(direction);
		while(!io_elevatorIsInFloor()){
			
		}
		io_stopMotor();
		floor=io_getCurrentFloor();
	}
}
void ctrl_checkSensor(){
	if(io_elevatorIsInFloor()){
		floor=io_getCurrentFloor();
		sm_handleEvent(FLOOR_REACHED);
	}
}	
/*
floorHasOrder()
og
noObstruction()
er guards for FSM
*/
int ctrl_floorHasOrder(){
	return (destinationMatrix[direction][floor] || destinationMatrix[COMMAND][floor]);
}
int ctrl_elevatorObstructed(){
	return io_elevatorIsObstructed();
}
/*
addOrderToList()
er en del av elevator-klassen
*/ 
void ctrl_addOrderToList(elev_button_type_t button, floor_t floor){
	destinationMatrix[button][floor]=1;
	io_setButtonLight(button, floor);		
}
/*
handleStop()
handleEmergencyStop()
handleDestination()
kalles av tilstandsmaskinen ved hhv ankomst etasje, nødstopp og avgang etasje
*/
void ctrl_handleStop(){
	ctrl_setLightsAtElevatorStop();
	clock_t startTime=clock();
	clock_t stopTime=clock();
	while( ((stopTime-startTime)/CLOCKS_PER_SEC) < 3){
		ui_checkButtons();
		if(ctrl_elevatorObstructed())
			startTime=stopTime;
		stopTime=clock();
	}
	io_closeDoor();	
	sm_handleEvent(NEW_DESTINATION);
}
void ctrl_handleEmergencyStop(){
	io_setStopLight();
	io_stopMotor();
	io_resetAllButtonLights();
	ctrl_clearDestinationMatrix();
}	
void ctrl_handleDestination(){
	io_resetStopLight();
	if(ctrl_checkOrderInThisDirection()){
		io_startMotor(direction);
	}else if(ctrl_checkOrderInOtherDirection()){
		io_startMotor((-1)*direction);
		direction = (-1)*direction;
	}
}
int ctrl_checkOrderInThisDirection(){
	int keepPreviousDirection=0;/*heisen går andre vei hvis ikke den får ordre i samme retning*/
	if(direction==DOWN)
		keepPreviousDirection=ctrl_checkLowerFloorsForOrders();
	else
		ctrl_checkUpperFloorsForOrders();
	return keepPreviousDirection;
}
int ctrl_checkOrderInOtherDirection(){
	int changeDirection=0;/*ĥeisen skal ikke endre retning dersom den ikke har ordre i andre retning*/
	if(direction==DOWN)
		changeDirection=ctrl_checkUpperFloorsForOrders();
	else
		ctrl_checkLowerFloorsForOrders();
	return changeDirection;
}
int ctrl_checkLowerFloorsForOrders(){
	int i,k;
	for(i=0;i<floor;i++){
		for(k=0;k<NUMBEROFBUTTONTYPES;k++){
			if(destinationMatrix[i][k]==1)
				return 1;
		}/* end k loop*/
	}/*end i loop*/
	return 0;
}
int ctrl_checkUpperFloorsForOrders(){
	int i,k;
	for(i=floor+1;i<NUMBEROFFLOORS;i++){
		for(k=0;k<NUMBEROFBUTTONTYPES;k++){
			if(destinationMatrix[i][k]==1)
				return 1;
		}/*end k loop*/
	}/*end i loop*/
	return 0;
}
void ctrl_clearDestinationMatrix(){
	int i,k;
	for(i=0;i<NUMBEROFBUTTONTYPES;i++){
		for(k=0;k<NUMBEROFFLOORS;k++){
			destinationMatrix[i][k]=0;
		}
	}
}
void ctrl_setLightsAtElevatorStop(){
	io_openDoor();
	io_resetButtonLight(BUTTON_COMMAND,floor);
	if(floor!=FIRST&&direction==DOWN)
		io_resetButtonLight(BUTTON_CALL_DOWN,floor);
	if(floor!=FOURTH&&direction==UP)
		io_resetButtonLight(BUTTON_CALL_UP,floor);
}	
