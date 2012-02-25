#include "elevator_ctrl.h"

direction_t direction=UP;
int currentFloor=-1;
int elevatorHasBeenObstructed=0;
direction_t lastDirection;
int orderMatrix[NUMBER_OF_BUTTON_TYPES][NUMBER_OF_FLOORS]={
         	              /*1	2	3	4*/
/*BUTTON_CALL_UP*/	{	0,	0,	0,	0},
/*BUTTON_CALL_DOWN*/	{	0,	0,	0,	0},
/*BUTTON_COMMAND*/	{	0,	0,	0,	0}
};

/*Hjelpefunksjoner, skal bort i endelig versjon*/
void potet(char str[]){
	printf("Potet: %s!\n",str);
}
void debug_printDestinationMatrix(){
        int i,k;
        printf("Floors:\t 1 \t 2 \t 3 \t 4 \t\n");
        for(i=0;i<NUMBER_OF_BUTTON_TYPES;i++){
                if(i==0)
                        printf("UP\t");
                else if(i==1)
                        printf("DOWN\t");
                else if(i==2)
                        printf("CMND\t");
                else
                        printf("Ukjent etasje. Erik har driti seg ut. Sjekk funksjonen..\n");
                for(k=0;k<NUMBER_OF_FLOORS;k++){
                        printf("%d\t",orderMatrix[i][k]);
                }
                printf("\n");
        }
}

/*Oppstartsfunksjon*/
void ctrl_initiateElevator(){
	if(io_elevatorIsInFloor()){
		currentFloor=io_getCurrentFloor();
		return;
	}
	else{
		direction=UP;
		io_startMotor();
		while(!io_elevatorIsInFloor()) {}
		io_stopMotor();
		currentFloor=io_getCurrentFloor();
	}
}

/*Get-funksjoner*/
direction_t ctrl_getDirection(){
	return direction;
}
int ctrl_elevatorHasBeenObstructed(){
	return elevatorHasBeenObstructed;
}

/*Actions*/
void ctrl_handleEmergencyStop(){
	io_setStopLight();
	timer_stopDoorTimer();
	io_stopMotorEmergency();
	ctrl_clearAllOrders();
}	
void ctrl_handleDestination(){
	ctrl_setNewDirection();
	lastDirection=direction;
	io_startMotor();
}
void ctrl_handleDestinationFromIdle(){
	if(ctrl_orderAtCurrentFloor())
		sm_handleEvent(FLOOR_REACHED);
	else{
		ctrl_setNewDirection();
		lastDirection=direction;
		io_startMotor();
	}
}
void ctrl_handleStop(){
	io_stopMotor();
	io_openDoor();
	ctrl_removeOrdersFromCurrentFloor();
}
void ctrl_handleDestinationFromEM(){
	io_closeDoor();
	io_resetStopLight();
	if(ctrl_orderAtCurrentFloor() && io_elevatorIsInFloor())
		sm_handleEvent(FLOOR_REACHED);
	else{
		ctrl_setNewDirection();
		io_startMotor();
	}
}
void ctrl_handleNewOrder(){
	order_t lastOrder=ui_lastOrder();
	orderMatrix[lastOrder.button][lastOrder.floor]=1;
	io_setButtonLight(lastOrder.button, lastOrder.floor);		
	sm_handleEvent(NEW_DESTINATION);
}

/*Guards*/
int ctrl_newOrderFromCommandButton(){
	order_t lastOrder=ui_lastOrder();
	return (lastOrder.button == BUTTON_COMMAND);	
}
int ctrl_newOrderNotInCurrentFloor(){
	order_t lastOrder=ui_lastOrder();
	return (lastOrder.floor != currentFloor);
}
int ctrl_noObstruction(){
	if(io_doorClosed()){
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
//TODO: Fjern denne. int ctrl_when_the_elevator_is_moving_should_it_stop_at_the_floor_the_elevator_is_currently_in()
int ctrl_stopElevatorAtCurrentFloor(){
	if(orderMatrix[BUTTON_COMMAND][currentFloor])
		return 1;
	if(direction==UP){
		if(orderMatrix[BUTTON_CALL_UP][currentFloor]){
			return 1;
		}else if(ctrl_upperFloorsHaveOrders()){
			return 0;
		}else
			return 1;
	}
	else if(direction==DOWN){
		if(orderMatrix[BUTTON_CALL_DOWN][currentFloor]){
			return 1;
		}else if(ctrl_lowerFloorsHaveOrders()){
			return 0;
		}else
			return 1;
	}
	return 0;
}

/*Lyttefunksjon*/
void ctrl_checkSensor(){
	int newFloor=io_getCurrentFloor();
	if(newFloor>=BOTTOM_FLOOR){
		currentFloor=newFloor;
		lastDirection=direction;
		io_setFloorIndicator(currentFloor);
		sm_handleEvent(FLOOR_REACHED);
	}
}	

/*Heisstyring*/
void ctrl_setNewDirection(){
	if(lastDirection==UP && ctrl_upperFloorsHaveOrders())
		direction=UP;
	else if(lastDirection==UP && ctrl_lowerFloorsHaveOrders())
		direction=DOWN;
	else if(lastDirection==DOWN && ctrl_lowerFloorsHaveOrders())
		direction=DOWN;
	else if(lastDirection==DOWN && ctrl_upperFloorsHaveOrders())
		direction=UP;
}
void ctrl_clearAllOrders(){
	io_resetAllButtonLights();
	int button,floor;
	for(floor=BOTTOM_FLOOR;floor<=TOP_FLOOR;floor++){
		for(button=0;button<NUMBER_OF_BUTTON_TYPES;button++){
			orderMatrix[button][floor]=0;
		}
	}
}
void ctrl_setLightsAtElevatorStop(){
	io_openDoor();
	io_resetButtonLight(BUTTON_COMMAND,currentFloor);
	if(currentFloor!=BOTTOM_FLOOR && direction==DOWN)
		io_resetButtonLight(BUTTON_CALL_DOWN,currentFloor);
	if(currentFloor!=TOP_FLOOR && direction==UP)
		io_resetButtonLight(BUTTON_CALL_UP,currentFloor);
}
void ctrl_removeOrdersFromCurrentFloor(){
	ctrl_removeSingleOrder(BUTTON_COMMAND);
	if(currentFloor==BOTTOM_FLOOR)
		ctrl_removeSingleOrder(BUTTON_CALL_UP);
	else if(currentFloor==TOP_FLOOR)
		ctrl_removeSingleOrder(BUTTON_CALL_DOWN);
	else{
		if(direction==UP){
			ctrl_removeSingleOrder(BUTTON_CALL_UP);
			if(!ctrl_upperFloorsHaveOrders())
				ctrl_removeSingleOrder(BUTTON_CALL_DOWN);
		}
		else{
			ctrl_removeSingleOrder(BUTTON_CALL_DOWN);
			if(!ctrl_lowerFloorsHaveOrders())
				ctrl_removeSingleOrder(BUTTON_CALL_UP);
		}	
	}
}
void ctrl_removeSingleOrder(buttonType_t button){
	orderMatrix[button][currentFloor]=0;
	io_resetButtonLight(button,currentFloor);
}
/*Sammenligning*/
int ctrl_orderAtCurrentFloor(){
	int button;
	for(button=0;button<NUMBER_OF_BUTTON_TYPES;button++){
		if(orderMatrix[button][currentFloor]) 
			return 1;
	}
	return 0;
}
int ctrl_lowerFloorsHaveOrders(){
	int floor,button,floorCorrection;
	if(lastDirection==UP)
		floorCorrection=1;
	else
		floorCorrection=0;
	for(floor=BOTTOM_FLOOR;floor<currentFloor+floorCorrection;floor++){
		for(button=0;button<NUMBER_OF_BUTTON_TYPES;button++){
			if(orderMatrix[button][floor]==1)
				return 1;
		}
	}
	return 0;
}
int ctrl_upperFloorsHaveOrders(){
	int floor,button,floorCorrection;
	if(lastDirection==UP)
		floorCorrection=1;
	else
		floorCorrection=0;
	for(floor=currentFloor+floorCorrection;floor<=TOP_FLOOR;floor++){
		for(button=0;button<NUMBER_OF_BUTTON_TYPES;button++){
			if(orderMatrix[button][floor]==1)
				return 1;
		}
	}
	return 0;
}
int ctrl_orderListHaveOrders(){
	int button,floor;
	for(button=0;button<NUMBER_OF_BUTTON_TYPES;button++){
		for(floor=BOTTOM_FLOOR;floor<=TOP_FLOOR;floor++){
			if(orderMatrix[button][floor])
				return 1;
		}
	}
	return 0;
}

