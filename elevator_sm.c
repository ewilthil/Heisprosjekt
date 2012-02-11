#include "elevator_sm.h"
#include "elevator_ctrl.h"

static enum state_t state = IDLE;

struct state_action_pair_t{
	enum state_t nextState;
	int (*guard)();	
	void (*action)();
};

/*nextState, guard, action */
struct state_action_pair_t stateTable[NUMBEROFSTATES][NUMBEROFEVENTS] = {
/*			NEW_DESTINATION						FLOOR_REACHED						STOP_PRESSED*/
/*EXECUTING_ORDER*/	{{EXECUTING_ORDER, NULL, NULL},				{TEMPORARY_STOP, ctrl_floorHasOrder, ctrl_handleStop},	{EMERGENCY_STOP, NULL, ctrl_handleEmergencyStop}	},
/*TEMPORARY_STOP*/	{{EXECUTING_ORDER, NULL, ctrl_handleDestination},	{TEMPORARY_STOP, NULL, ctrl_handleStop},		{EMERGENCY_STOP, NULL, ctrl_handleEmergencyStop}	},
/*IDLE*/		{{EXECUTING_ORDER, NULL, ctrl_handleDestination}, 	{IDLE, NULL, NULL},					{EMERGENCY_STOP, NULL, ctrl_handleEmergencyStop}	},
/*EMERGENCY_STOP*/	{{EXECUTING_ORDER, NULL, ctrl_handleDestination}, 	{EMERGENCY_STOP, NULL, NULL},				{EMERGENCY_STOP, NULL, ctrl_handleEmergencyStop}	}
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

