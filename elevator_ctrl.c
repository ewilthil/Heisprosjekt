#include "elevator_io.h"
#include "elevator_ctrl.h"
#include "elevator_sm.h"
#include "elevator_ui.h"
#include <stdio.h>
//#include "avlusing.h"

direction_t direction=UP;
int floor=-1;
int destinationMatrix[NUMBEROFBUTTONTYPES][NUMBEROFFLOORS]={
                      /*1	2	3	4*/
/*CALL_UP*/{		0,	0,	0,	0},
/*CALL_DOWN*/{		0,	0,	0,	0},
/*COMMAND*/{		0,	0,	0,	0}
};

void debug_printDestinationMatrix(){
        int i,k;
        printf("Floors:\t 1 \t 2 \t 3 \t 4 \t\n");
        for(i=0;i<NUMBEROFBUTTONTYPES;i++){
                if(i==0)
                        printf("UP\t");
                else if(i==1)
                        printf("DOWN\t");
                else if(i==2)
                        printf("CMND\t");
                else
                        printf("Ukjent etasje. Erik har driti seg ut. Sjekk funksjonen..\n");
                for(k=0;k<NUMBEROFFLOORS;k++){
                        printf("%d\t",destinationMatrix[i][k]);
                }
                printf("\n");
        }
}


void ctrl_initiateElevator(){
	if(io_elevatorIsInFloor()){
		floor=io_getCurrentFloor();
		return;
	}
	else{
		direction=UP;
		io_startMotor();
		while(!io_elevatorIsInFloor()){
			
		}
		io_stopMotor();
		floor=io_getCurrentFloor();
	}
}
void ctrl_checkSensor(){
	floor=io_getCurrentFloor();
	if(floor!=-1){
		sm_handleEvent(FLOOR_REACHED);
	}
}	
/*
 * floorHasOrder() og noObstruction() er guards for FSM
 */
int ctrl_floorHasOrder(){
	if(destinationMatrix[COMMAND][floor])
		return 1;
	if(direction==UP){
		if(destinationMatrix[BUTTON_CALL_UP][floor]){
			return 1;
		}else if(ctrl_checkUpperFloorsForOrders()){
			return 0;
		}
	}
	else if(direction==DOWN){
		if(destinationMatrix[BUTTON_CALL_DOWN][floor]){
			return 1;
		}else if(ctrl_checkLowerFloorsForOrders()){
			return 0;
		}
	}
	return 0;
}
int ctrl_elevatorObstructed(){
	return io_elevatorIsObstructed();
}
void ctrl_addOrderToList(elev_button_type_t button, int floor){
	if(state != EMERGENCY_STOP || button == BUTTON_COMMAND){
		destinationMatrix[button][floor]=1;
		io_setButtonLight(button, floor);
		sm_handleEvent(NEW_DESTINATION);
	}
}
/*
 * handleStop()
 * handleEmergencyStop()
 * handleDestination()
 * kalles av tilstandsmaskinen ved hhv ankomst etasje, nødstopp og avgang etasje
 */
void ctrl_handleStop(){
	io_stopMotor();
	io_openDoor();
	ctrl_removeOrder();
	ctrl_setLightsAtElevatorStop();
	clock_t startTime=clock();
	clock_t stopTime=clock();
	while( ((stopTime-startTime)/CLOCKS_PER_SEC) < 3){
		ui_checkStop();
		ui_checkButtons();
		if(ctrl_elevatorObstructed())
			startTime=stopTime;
		stopTime=clock();
	}
	io_closeDoor();
	if(!ctrl_orderListHasOrders()){	
		sm_handleEvent(NEW_DESTINATION);
	}else{
		sm_handleEvent(FLOOR_REACHED);
	}
}
void ctrl_handleEmergencyStop(){
	io_setStopLight();
	io_stopMotor();
	io_resetAllButtonLights();
	ctrl_clearDestinationMatrix();
}	
void ctrl_handleDestination(){
	io_resetStopLight();
	ctrl_setNewDirection();
	io_startMotor();
}
void  ctrl_setNewDirection(){
	debug_printDestinationMatrix();
	printf("Direction: ");
	if(direction==UP && ctrl_checkUpperFloorsForOrders()){
		direction=UP;
		printf("Case 1: UP\n\n");
	}
	else if(direction==UP && ctrl_checkLowerFloorsForOrders()){
		direction=DOWN;
		printf("Case 2: DOWN\n\n");
	}
	else if(direction==DOWN && ctrl_checkLowerFloorsForOrders()){
		direction=DOWN;
		printf("Case 3: DOWN\n\n");
	}
	else if(direction==DOWN && ctrl_checkUpperFloorsForOrders()){
		direction=UP;
		printf("Case 4: UP\n\n");
	}
	else
		printf("Feil med ordreliste eller noe...\n");		
}

int ctrl_checkLowerFloorsForOrders(){
	int i,k,dir;
	if(direction==UP)
		dir=1;
	else
		dir=0;
	for(i=0;i<floor+dir;i++){
		for(k=0;k<NUMBEROFBUTTONTYPES;k++){
			if(destinationMatrix[k][i]==1){
				printf("Lower: button,floor: %d,%d = %d\n",k,i,destinationMatrix[k][i]);
				return 1;
			}
		}/* end k loop*/
	}/*end i loop*/
	return 0;
}
int ctrl_checkUpperFloorsForOrders(){
	int i,k,dir;
	
	if(direction==UP)
		dir=1;
	else
		dir=0;
	for(i=floor+dir;i<NUMBEROFFLOORS;i++){
		for(k=0;k<NUMBEROFBUTTONTYPES;k++){
			if(destinationMatrix[k][i]==1){
				printf("Upper: button,floor: %d,%d = %d\n",k,i,destinationMatrix[k][i]);
				return 1;
			}
		}/*end k loop*/
	}/*end i loop*/
	return 0;
}
void ctrl_clearDestinationMatrix(){
	int i,k;
	for(i=0;i<NUMBEROFFLOORS;i++){
		for(k=0;k<NUMBEROFBUTTONTYPES;k++){
			destinationMatrix[k][i]=0;
		}
	}
}
void ctrl_setLightsAtElevatorStop(){
	io_openDoor();
	io_resetButtonLight(BUTTON_COMMAND,floor);
	if(floor!=0 && direction==DOWN){
		io_resetButtonLight(BUTTON_CALL_DOWN,floor);
	}
	if(floor!=3&&direction==UP)
		io_resetButtonLight(BUTTON_CALL_UP,floor);
}
void ctrl_removeOrder(){
	destinationMatrix[BUTTON_COMMAND][floor]=0;
	io_resetButtonLight(BUTTON_COMMAND,floor);
	if(floor!=0 && direction==UP){
		destinationMatrix[BUTTON_CALL_UP][floor]=0;
		io_resetButtonLight(BUTTON_CALL_UP,floor);
		if(!ctrl_checkUpperFloorsForOrders()){
			destinationMatrix[BUTTON_CALL_DOWN][floor]=0;
			io_resetButtonLight(BUTTON_CALL_DOWN,floor);
		}
	}
	if(floor!=3 && direction==DOWN){
		destinationMatrix[BUTTON_CALL_DOWN][floor]=0;
		io_resetButtonLight(BUTTON_CALL_DOWN,floor);
		if(!ctrl_checkLowerFloorsForOrders()){
			destinationMatrix[BUTTON_CALL_UP][floor]=0;
			io_resetButtonLight(BUTTON_CALL_UP,floor);
		}
	}
}
int ctrl_orderListHasOrders(){
	int i,j;
	for(i=0;i<NUMBEROFBUTTONTYPES;i++){
		for(j=0;j<NUMBEROFFLOORS;j++){
			if(destinationMatrix[i][j])
				return 1;

		}
	}
	return 0;
}	
