#include "elevator_ctrl.h"

direction_t direction=UP;
//TODO: Antar floor også er standardfunksjon, bytt navn
int doorClosed=1;
int currentFloor=-1;
int elevatorHasBeenObstructed=0;
int destinationMatrix[NUMBEROFBUTTONTYPES][NUMBEROFFLOORS]={
                      /*1	2	3	4*/
/*CALL_UP*/{		0,	0,	0,	0},
/*CALL_DOWN*/{		0,	0,	0,	0},
/*COMMAND*/{		0,	0,	0,	0}
};
void potet(char str[]){
	printf("Potet: %s!\n",str);
}
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


direction_t ctrl_getDirection(){
	return direction;
}

void ctrl_initiateElevator(){
	if(io_elevatorIsInFloor()){
		currentFloor=io_getCurrentFloor();
		return;
	}
	else{
		direction=UP;
		io_startMotor();
		while(!io_elevatorIsInFloor()){
			
		}
		io_stopMotor();
		currentFloor=io_getCurrentFloor();
	}
}
void ctrl_checkSensor(){
	int newFloor=io_getCurrentFloor();
	if(newFloor!=-1){
		currentFloor=newFloor;
		io_setFloorIndicator(currentFloor);
		sm_handleEvent(FLOOR_REACHED);
	}
}	
/*
 * floorHasOrder() og noObstruction() er guards for FSM
 */
int ctrl_floorHasOrder(){
	if(destinationMatrix[COMMAND][currentFloor]){
		return 1;
	}
	if(direction==UP){
		if(destinationMatrix[BUTTON_CALL_UP][currentFloor]){
			return 1;
		}else if(ctrl_checkUpperFloorsForOrders()){
			return 0;
		}else
			return 1;
	}
	else if(direction==DOWN){
		if(destinationMatrix[BUTTON_CALL_DOWN][currentFloor]){
			return 1;
		}else if(ctrl_checkLowerFloorsForOrders()){
			return 0;
		}else
			return 1;
	}
	return 0;
}
int ctrl_doorClosed(){
	return (doorClosed);
}
int ctrl_orderTypeCommand(){
	order_t lastOrder=ui_lastOrder();
	return (lastOrder.button == BUTTON_COMMAND);	
}
int ctrl_orderNotInCurrentFloor(){
	order_t lastOrder=ui_lastOrder();
	return (lastOrder.floor != currentFloor);
}
int ctrl_orderAtCurrentFloor(){
	int i;
	for(i=0;i<NUMBEROFBUTTONTYPES;i++){
		if(destinationMatrix[i][currentFloor])
			return 1;
	}
	return 0;
}
void ctrl_addOrderToList(){
	order_t lastOrder=ui_lastOrder();
	destinationMatrix[lastOrder.button][lastOrder.floor]=1;
	io_setButtonLight(lastOrder.button, lastOrder.floor);		
	sm_handleEvent(NEW_DESTINATION);
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
	clock_t startTime=clock();
	clock_t stopTime=clock();
	while( ((stopTime-startTime)/CLOCKS_PER_SEC) < 3){
		ui_checkStop();
		ui_checkButtons();
		ui_checkObstruction();
		if(io_motorIsRunning())
			return;
		if(io_elevatorIsObstructed())
			startTime=stopTime;
		stopTime=clock();
	}
	state_t state = sm_getState();
	if(state==EMERGENCY_STOP)
		return;
	io_closeDoor();
	if(ctrl_orderListHasOrders()){
		sm_handleEvent(NEW_DESTINATION);
	}else{
		sm_handleEvent(FLOOR_REACHED);
	}
}
void ctrl_handleEmergencyStop(){
	io_setStopLight();
	io_stopMotorEmergency();
	io_resetAllButtonLights();
	ctrl_clearDestinationMatrix();
}	
void ctrl_handleDestination(){
	ctrl_setNewDirection();
	io_startMotor();
}
void ctrl_handleDestinationFromIdle(){
	if(ctrl_orderAtCurrentFloor()){
		sm_handleEvent(FLOOR_REACHED);
	}
	else{
		ctrl_setNewDirection();
		io_startMotor();
	}
}
void ctrl_handleDestinationFromEM(){
	io_closeDoor();
	io_resetStopLight();
	if(ctrl_orderAtCurrentFloor() && io_elevatorIsInFloor()){
		sm_handleEvent(FLOOR_REACHED);
	}
	else{
		ctrl_setNewDirection();
		io_startMotor();
	}
}
/*
void ctrl_goToOrder(){
	io_resetStopLight();
	
}*/
void  ctrl_setNewDirection(){
	if(direction==UP && ctrl_checkUpperFloorsForOrders()){
		direction=UP;
	}
	else if(direction==UP && ctrl_checkLowerFloorsForOrders()){
		direction=DOWN;
	}
	else if(direction==DOWN && ctrl_checkLowerFloorsForOrders()){
		direction=DOWN;
	}
	else if(direction==DOWN && ctrl_checkUpperFloorsForOrders()){
		direction=UP;
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
	for(i=0;i<currentFloor+dir;i++){
		for(k=0;k<NUMBEROFBUTTONTYPES;k++){
			if(destinationMatrix[k][i]==1){
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
	for(i=currentFloor+dir;i<NUMBEROFFLOORS;i++){
		for(k=0;k<NUMBEROFBUTTONTYPES;k++){
			if(destinationMatrix[k][i]==1){
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
	io_resetButtonLight(BUTTON_COMMAND,currentFloor);
	if(currentFloor!=0 && direction==DOWN){
		io_resetButtonLight(BUTTON_CALL_DOWN,currentFloor);
	}
	if(currentFloor!=3&&direction==UP)
		io_resetButtonLight(BUTTON_CALL_UP,currentFloor);
}
void ctrl_removeOrder(){
	destinationMatrix[BUTTON_COMMAND][currentFloor]=0;
	io_resetButtonLight(BUTTON_COMMAND,currentFloor);
	//NB: Det var originalt byttet om på 3 og 0, tror dette blir riktig, men det kan være noe jeg ikke har tenkt på
	if(currentFloor!=3 && currentFloor!=0 && direction==UP){
		destinationMatrix[BUTTON_CALL_UP][currentFloor]=0;
		io_resetButtonLight(BUTTON_CALL_UP,currentFloor);
		if(!ctrl_checkUpperFloorsForOrders()){
			destinationMatrix[BUTTON_CALL_DOWN][currentFloor]=0;
			io_resetButtonLight(BUTTON_CALL_DOWN,currentFloor);
		}
	}
	if(currentFloor!=0 && currentFloor!=3 && direction==DOWN){
		destinationMatrix[BUTTON_CALL_DOWN][currentFloor]=0;
		io_resetButtonLight(BUTTON_CALL_DOWN,currentFloor);
		if(!ctrl_checkLowerFloorsForOrders()){
			destinationMatrix[BUTTON_CALL_UP][currentFloor]=0;
			io_resetButtonLight(BUTTON_CALL_UP,currentFloor);
		}
	}
	if(currentFloor==0){
		destinationMatrix[BUTTON_CALL_UP][currentFloor]=0;
		io_resetButtonLight(BUTTON_CALL_UP,currentFloor);
	}
	if(currentFloor==3){
		destinationMatrix[BUTTON_CALL_DOWN][currentFloor]=0;
		io_resetButtonLight(BUTTON_CALL_DOWN,currentFloor);
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
int ctrl_noObstruction(){
	
	if(doorClosed){
		elevatorHasBeenObstructed=0;
		return 1;
	}
	else if(io_elevatorIsObstructed()){
		elevatorHasBeenObstructed=1;
		return 0;
	}
	else{
		elevatorHasBeenObstructed=0;
		return 1;
		
	}
}
