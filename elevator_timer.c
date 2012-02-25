#include "elevator_timer.h"

int timerActive=0;
clock_t startClock;
clock_t currentClock;


void timer_startDoorTimer(){
	timerActive=1;
	startClock=clock();
}
void timer_checkDoorTimer(){
	if(timerActive){
		currentClock=clock();
		if(io_elevatorIsObstructed())
			startClock=currentClock;
		if(((currentClock-startClock)/CLOCKS_PER_SEC) > 3){
			timer_stopDoorTimer();
			io_closeDoor();
			if(ctrl_orderListHaveOrders())
				sm_handleEvent(NEW_DESTINATION);
			else
				sm_handleEvent(FLOOR_REACHED);
		}
	}
}

void timer_stopDoorTimer(){
	timerActive=0;
}

