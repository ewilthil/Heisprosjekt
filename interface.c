#include "elevator.h"
#include "tester.h"
#include "elev.h"

void checkStop(){
	if(elev_get_stop_signal()){
		handleEvent(STOP_PRESSED);
	}	
}

void checkButtons(){
	elev_button_type_t buttonType;
	floor_t floor;
	
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
