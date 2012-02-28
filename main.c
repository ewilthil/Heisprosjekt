#include "elev.h"
#include "elevator_ui.h"
#include "elevator_ctrl.h"
#include "elevator_timer.h"
#include <stdio.h>

int main()
{
    	// Initialize hardware
   	if (!elev_init()) {
        	printf(__FILE__ ": Unable to initialize elevator hardware\n");
        	return 1;
   	}
	
	ctrl_initiateElevator();
    	while (1) {
		ui_checkStop();
		ui_checkButtons();
		ctrl_checkSensor();
		ui_checkObstruction();
		timer_checkDoorTimer();
       	}

    	return 0;
}

