#include "elevator_ui.h"
#include <stdlib.h>
static EMfromButton=1;
static order_t lastOrder;
static int elevatorHasBeenObstructed=0;
order_t ui_lastOrder(){
	return lastOrder;
}
void ui_checkStop(){
	if(io_emergencyStopPressed()){
		EMfromButton=1;
		sm_handleEvent(STOP_PRESSED);
	}	
}
void ui_checkButtons(){
	if(io_emergencyStopPressed())
		return;
	buttonType_t buttonType;
	int floor;

	buttonType = BUTTON_CALL_UP;
	for(floor=0; floor<=2; floor++){
		if(io_orderButtonPressed(buttonType, floor)){	
			lastOrder.button=buttonType;
			lastOrder.floor=floor;
			sm_handleEvent(ORDER_BUTTON_PRESSED);
		}
	}

	buttonType = BUTTON_CALL_DOWN;
	for(floor=1; floor<=3; floor++){
		if(io_orderButtonPressed(buttonType, floor)){	
			lastOrder.button=buttonType;
			lastOrder.floor=floor;
			sm_handleEvent(ORDER_BUTTON_PRESSED);
		}
	}

	buttonType = BUTTON_COMMAND;
	for(floor=0; floor<=3; floor++){
		if(io_orderButtonPressed(buttonType, floor)){	
			lastOrder.button=buttonType;
			lastOrder.floor=floor;
			sm_handleEvent(ORDER_BUTTON_PRESSED);
		}
	}
}
//TODO:Endre funksjon, den suger
void ui_checkObstruction(){
	
	//if(ctrl_noObstruction())
	int elevatorIsObstructed=io_elevatorIsObstructed();
	if(ui_obstructionIsRemoved(elevatorIsObstructed) && ctrl_orderListHaveOrders())
		sm_handleEvent(NEW_DESTINATION);
	if(io_elevatorIsObstructed()){
		if(io_motorIsRunning()){
			EMfromButton=0;
			sm_handleEvent(STOP_PRESSED);
		}
	}
	elevatorHasBeenObstructed=elevatorIsObstructed;
}
int ui_obstructionIsRemoved(int elevatorIsObstructed){
	if(elevatorIsObstructed==0 && elevatorHasBeenObstructed==1){
		potet("obsRem");
		return 1;
	}
	else
		return 0;
}
