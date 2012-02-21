#ifndef ELEVATOR_UI_H
#define ELEVATOR_UI_H
//TODO: bytt fra elev til io
//#include "elevator_io.h"
#include "elevator_sm.h"
#include "elevator_ctrl.h"
#include "elev.h"
#define buttonType_t elev_button_type_t
typedef struct{
	buttonType_t button;
	int floor;
}order_t;

order_t ui_lastOrder();
void ui_checkStop();
void ui_checkButtons();
void ui_checkObstruction();

#endif 
