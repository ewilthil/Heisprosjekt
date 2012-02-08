#include "elevatorFSM.h"
#include "elevator.h"

static enum state_t state = IDLE;

struct state_action_pair_t{
	enum state_t nextState;
	void (*action)();
	int (*guard)();	
};

/*nextState, action, guard*/
struct state_action_pair_t stateTable[NUMBEROFSTATES][NUMBEROFEVENTS] = {
/*			NEW_DESTINATION					FLOOR_REACHED					STOP_PRESSED*/
/*EXECUTING_ORDER*/	{{EXECUTING_ORDER, NULL, NULL},			{TEMPORARY_STOP, floorHasOrders, handleStop},	{EMERGENCY_STOP, NULL, handleEmergencyStop}	},
/*TEMPORARY_STOP*/	{{EXECUTING_ORDER, NULL, handleDestination},	{TEMPORARY_STOP, NULL, handleStop},		{EMERGENCY_STOP, NULL, handleEmergencyStop}	},
/*IDLE*/		{{EXECUTING_ORDER, NULL, handleDestination}, 	{IDLE, NULL, NULL},				{EMERGENCY_STOP, NULL, handleEmergencyStop}	},
/*EMERGENCY_STOP*/	{{EXECUTING_ORDER, NULL, handleDestination}, 	{EMERGENCY_STOP, NULL, NULL},			{EMERGENCY_STOP, NULL, handleEmergencyStop}	}
};

void handleEvent(enum event_t event){
	//TODO: bytt ut sap med noe annet (Dette er kun internt i fila)
	struct state_action_pair_t sap = stateTable[state][event];
	if(sap.guard == NULL || sap.guard()){
		if(sap.action != NULL){
			sap.action();
		}
		state = sap.nextState;
	}
}

