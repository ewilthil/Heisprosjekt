#include "elevator_io.h"
#include "elevator_ctrl.h"
#include "elevator_sm.h"
#include "elevator_ui.h"
#include <stdio.h>
//#include "avlusing.h"


static floor_t floor;
//direction_t direction = UP; Deklareres i elevator_ctrl.h
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
 * floorHasOrder() og noObstruction() er guards for FSM
 */
int ctrl_floorHasOrder(){
	return (destinationMatrix[direction][floor] || destinationMatrix[COMMAND][floor]);
}
int ctrl_elevatorObstructed(){
	return io_elevatorIsObstructed();
}
void ctrl_addOrderToList(elev_button_type_t button, floor_t floor){
	destinationMatrix[button][floor]=1;
	io_setButtonLight(button, floor);
	sm_handleEvent(NEW_DESTINATION);		
}
/*
 * handleStop()
 * handleEmergencyStop()
 * handleDestination()
 * kalles av tilstandsmaskinen ved hhv ankomst etasje, nødstopp og avgang etasje
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
	printf("Retning: %d\n",direction);
	debug_printDestinationMatrix();
	ctrl_setNewDirection();
	io_startMotor();
	/*
	if(ctrl_checkOrderInCurrentDirection()){
		printf("Motor burde startes\n");
		io_startMotor(direction);
	}else if(ctrl_checkOrderInOtherDirection()){
		io_startMotor((-1)*direction);
		direction = (-1)*direction;
	}
	*/
}
void ctrl_setNewDirection(){
	if(ctrl_checkForOrdersInCurrentDirection()){
		return;
	}else if(ctrl_checkForOrdersInOppositeDirection()){
		if(direction==UP)
			direction=DOWN;
		else
			direction=UP;
	}
}

int ctrl_checkForOrdersInCurrentDirection(){
	int ordersInCurrentDirection;/*heisen går andre vei hvis ikke den får ordre i samme retning*/
	if(direction==DOWN){
		ordersInCurrentDirection=ctrl_checkLowerFloorsForOrders();
	}
	else{
		ordersInCurrentDirection=ctrl_checkUpperFloorsForOrders();
	}		
	return ordersInCurrentDirection;
}
int ctrl_checkForOrdersInOppositeDirection(){
	int ordersInOppositeDirection;/*heisen går andre vei hvis ikke den får ordre i samme retning*/
        if(direction==DOWN){
                ordersInOppositeDirection=ctrl_checkUpperFloorsForOrders();
        }
        else{
                ordersInOppositeDirection=ctrl_checkLowerFloorsForOrders();
        } 
        return ordersInOppositeDirection;
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
