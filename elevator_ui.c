#include "elevator_ui.h"
#include <stdlib.h>

static order_t lastOrder;
static int elevatorHasBeenObstructed=0;
order_t ui_lastOrder(){
	return lastOrder;
}
void ui_checkStop(){
	if(io_emergencyStopPressed()){
		sm_handleEvent(STOP_PRESSED);
	}	
}
void ui_checkButtons(){
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
	if(ui_obstructionIsRemoved() && ctrl_orderListHaveOrders())
		sm_handleEvent(NEW_DESTINATION);
	if(io_elevatorIsObstructed()){
		if(io_motorIsRunning())
			sm_handleEvent(STOP_PRESSED);
	}
	elevatorHasBeenObstructed=io_elevatorIsObstructed();
}
int ui_obstructionIsRemoved(){
	printf("%d\n",io_elevatorIsObstructed());
	if(io_elevatorIsObstructed()==0 && elevatorHasBeenObstructed==1){
		potet("obsRem");
		exit(1);
		return 1;
	}
	else
		return 0;
}
