#include "elevator_sm.h"
#include "elevator_ctrl.h"
#include <stdio.h>

state_t state = IDLE;

struct state_action_pair_t{
	state_t nextState;
	int (*guard)();	
	void (*action)();
};

/*nextState, guard, action */
struct state_action_pair_t stateTable[NUMBEROFSTATES][NUMBEROFEVENTS] = {
/*			NEW_DESTINATION						FLOOR_REACHED						STOP_PRESSED*/
/*EXECUTING_ORDER*/	{{EXECUTING_ORDER, NULL, NULL},				{TEMPORARY_STOP, ctrl_floorHasOrder, ctrl_handleStop},	{EMERGENCY_STOP, NULL, ctrl_handleEmergencyStop}	},
/*TEMPORARY_STOP*/	{{EXECUTING_ORDER, NULL, ctrl_handleDestination},	{IDLE, NULL, NULL},					{EMERGENCY_STOP, NULL, ctrl_handleEmergencyStop}	},
/*IDLE*/		{{EXECUTING_ORDER, NULL, ctrl_handleDestination}, 	{IDLE, NULL, NULL},					{EMERGENCY_STOP, NULL, ctrl_handleEmergencyStop}	},
/*EMERGENCY_STOP*/	{{EXECUTING_ORDER, NULL, ctrl_handleDestination}, 	{EMERGENCY_STOP, NULL, NULL},				{EMERGENCY_STOP, NULL, ctrl_handleEmergencyStop}	}
};

//TODO:Det bør bære en guard på New_Des->Temp, hvis ikke får man overgang på første addOrderToList!!!!

void sm_handleEvent(event_t event){
	struct state_action_pair_t transition = stateTable[state][event];
	if(transition.guard == NULL || transition.guard()){
		state = transition.nextState; //Denne stod som vist under
		if(transition.action != NULL){
			transition.action();
		}
		//state = transition.nextState;
	}
}

void sm_printState(){
	switch(state){
		case IDLE: printf("IDLE\n"); break;
		case EXECUTING_ORDER: printf("EXECUTING_ORDER\n"); break;
		case TEMPORARY_STOP: printf("TEMPORARY_STOP\n"); break;
		case EMERGENCY_STOP: printf("EMERGENCY_STOP\n"); break;
	}
}
