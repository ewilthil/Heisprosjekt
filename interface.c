#include "main.h"
#include "elev.h"

void checkStop(){
	if(elev_get_stop_signal()){
		handleEvent(STOP_PRESSED);
	}	
	return;
}

void checkButtons(){
	elev_button_type_t buttonType;
	int floor;
	
	buttonType = BUTTON_CALL_UP;
	for(floor=0; floor<=2; floor++){
		if(elev_get_button_signal(buttonType, floor)){	
			addOrderToList(floor,buttonType); 
		}
	}

	buttonType = BUTTON_CALL_DOWN;
	for(floor=1; floor<=3; floor++){
		if(elev_get_button_signal(buttonType, floor)){	
			addOrderToList(floor,buttonType); 
		}
	}

	buttonType = BUTTON_COMMAND;
	for(floor=0; floor<=3; floor++){
		if(elev_get_button_signal(buttonType, floor)){	
			addOrderToList(floor,buttonType); 
		}
	}
}
