#include "elevator_ui.h"

order_t lastOrder;

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
void ui_checkObstruction(){
	if(ctrl_elevatorHasBeenObstructed())
		sm_handleEvent(NEW_DESTINATION);
}
