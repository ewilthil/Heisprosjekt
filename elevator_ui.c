#include "elevator_ui.h"
#include <stdlib.h>

static order_t lastOrder;
static int elevatorHasBeenObstructed=0;

order_t ui_getLastOrder(){
	return lastOrder;
}
void ui_checkStop(){
	if(io_emergencyStopPressed())
		sm_handleEvent(STOP_PRESSED);
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
void ui_checkObstruction(){
	//Det er nødvendig å sette en variabel lik io_elevatorIsObstructed() fordi den kan gi ulike verdier dersom den blir kalt to ganger etter hverandre
	int elevatorIsObstructed=io_elevatorIsObstructed();
	if(ui_obstructionIsRemoved(elevatorIsObstructed) && ctrl_orderListHaveOrders())
		sm_handleEvent(NEW_DESTINATION);
	if(io_elevatorIsObstructed() && io_motorIsRunning())
		sm_handleEvent(STOP_PRESSED);
	elevatorHasBeenObstructed=elevatorIsObstructed;
}
int ui_obstructionIsRemoved(int elevatorIsObstructed){
	if(elevatorIsObstructed==0 && elevatorHasBeenObstructed==1)
		return 1;
	else
		return 0;
}
