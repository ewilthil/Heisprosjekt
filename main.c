// REPLACE THIS FILE WITH YOUR OWN CODE.
// READ ELEV.H FOR INFORMATION ON HOW TO USE THE ELEVATOR FUNCTIONS.

#include "elev.h"
#include "elevator_ui.h"
#include "elevator_ctrl.h"
#include <stdio.h>
#include "avlusing.h"

int main()
{
    	// Initialize hardware
   	if (!elev_init()) {
        	printf(__FILE__ ": Unable to initialize elevator hardware\n");
        	return 1;
   	}
    	
	//debug_testDestinationMatrix();
	
	ctrl_initiateElevator();
    	while (1) {
		ui_checkStop();
		ui_checkButtons();
		ctrl_checkSensor();
		ui_checkObstruction();
       	}
	

    	return 0;
}

