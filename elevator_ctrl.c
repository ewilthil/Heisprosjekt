#include "elevator_ctrl.h"
#include "elevator_ctrl_private.h"

static direction_t direction=UP;
static int currentFloor=-1;
static direction_t directionFromLastFloor=UP;
static int orderMatrix[NUMBER_OF_BUTTON_TYPES][NUMBER_OF_FLOORS]={
         	              /*1	2	3	4*/
/*BUTTON_CALL_UP*/	{	0,	0,	0,	0},
/*BUTTON_CALL_DOWN*/	{	0,	0,	0,	0},
/*BUTTON_COMMAND*/	{	0,	0,	0,	0}
};

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
direction_t ctrl_getDirection(){
	return direction;
}
void ctrl_checkSensor(){
	int newFloor=io_getCurrentFloor();
	if(newFloor>=BOTTOM_FLOOR){
		currentFloor=newFloor;
		directionFromLastFloor=direction;
		io_setFloorIndicator(currentFloor);
		sm_handleEvent(FLOOR_REACHED);
	}
}

/*Handlinger for tilstandsmaskin*/
void ctrl_handleEmergencyStop(){
	io_setStopLight();
	io_stopMotor();
	ctrl_clearAllOrders();
}	
void ctrl_handleDestination(){
	ctrl_setNewDirection();
	directionFromLastFloor=direction;
	io_startMotor();
}
void ctrl_handleDestinationFromIdle(){
	if(ctrl_orderAtCurrentFloor())
		sm_handleEvent(FLOOR_REACHED);
	else{
		ctrl_setNewDirection();
		directionFromLastFloor=direction;
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

/*Betingelser for tilstandsmaskin*/
int ctrl_newOrderFromCommandButton(){
	order_t lastOrder=ui_lastOrder();
	return (lastOrder.button == BUTTON_COMMAND);	
}
int ctrl_newOrderNotInCurrentFloor(){
	order_t lastOrder=ui_lastOrder();
	return (lastOrder.floor != currentFloor);
}
int ctrl_noObstruction(){
	return !io_elevatorIsObstructed();
}
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

/*Støttefunksjoner for handlinger og betingelser*/
static void ctrl_setNewDirection(){
	if(directionFromLastFloor==UP && ctrl_upperFloorsHaveOrders())
		direction=UP;
	else if(directionFromLastFloor==UP && ctrl_lowerFloorsHaveOrders())
		direction=DOWN;
	else if(directionFromLastFloor==DOWN && ctrl_lowerFloorsHaveOrders())
		direction=DOWN;
	else if(directionFromLastFloor==DOWN && ctrl_upperFloorsHaveOrders())
		direction=UP;
}
static void ctrl_clearAllOrders(){
	io_resetAllButtonLights();
	int button,floor;
	for(floor=BOTTOM_FLOOR;floor<=TOP_FLOOR;floor++){
		for(button=0;button<NUMBER_OF_BUTTON_TYPES;button++){
			orderMatrix[button][floor]=0;
		}
	}
}
static void ctrl_removeOrdersFromCurrentFloor(){
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
static void ctrl_removeSingleOrder(buttonType_t button){
	orderMatrix[button][currentFloor]=0;
	io_resetButtonLight(button,currentFloor);
}
static int ctrl_orderAtCurrentFloor(){
	int button;
	for(button=0;button<NUMBER_OF_BUTTON_TYPES;button++){
		if(orderMatrix[button][currentFloor]) 
			return 1;
	}
	return 0;
}
static int ctrl_lowerFloorsHaveOrders(){
	int floor,button,floorCorrection;
	if(directionFromLastFloor==UP)
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
static int ctrl_upperFloorsHaveOrders(){
	int floor,button,floorCorrection;
	if(directionFromLastFloor==UP)
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
