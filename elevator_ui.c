#include "elevator_ctrl.h"
#include "elev.h"
#include "elevator_sm.h"

void ui_checkStop(){
	if(elev_get_stop_signal()){
		sm_handleEvent(STOP_PRESSED);
	}	
}

void ui_checkButtons(){
	elev_button_type_t buttonType;
	int floor;
	
	buttonType = BUTTON_CALL_UP;
	for(floor=0; floor<=2; floor++){
		if(elev_get_button_signal(buttonType, floor)){	
			ctrl_addOrderToList(buttonType,floor); 
		}
	}

	buttonType = BUTTON_CALL_DOWN;
	for(floor=1; floor<=3; floor++){
		if(elev_get_button_signal(buttonType, floor)){	
			ctrl_addOrderToList(buttonType,floor); 
		}
	}

	buttonType = BUTTON_COMMAND;
	for(floor=0; floor<=3; floor++){
		if(elev_get_button_signal(buttonType, floor)){	
			ctrl_addOrderToList(buttonType,floor); 
		}
	}
}
