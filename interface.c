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
	
	for(floor=0; floor<=3; floor++){
		//Mulig aa traversere en enum direkte?
		buttonType = BUTTON_CALL_UP;
		if(elev_get_button_signal(buttonType, floor)){
			//TODO: oversette int floor til enum
			//Det er ikke sikkert losningen som er atm vil fungere
			//staticcast?
			addOrderToList(floor,buttonType); 
		}
		buttonType = BUTTON_CALL_DOWN;
		if(elev_get_button_signal(buttonType, floor)){
                    	 addOrderToList(floor,buttonType);
                }
		buttonType = BUTTON_COMMAND;
		if(elev_get_button_signal(buttonType, floor)){
			addOrderToList(floor, buttonType);
		}
	}
}
