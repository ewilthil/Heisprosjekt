/*
Spørsmål eller kommentarer?
*/
#ifndef MAIN_H
#define MAIN_H

#include <time.h>

#define NUMBEROFFLOORS 4 
#define NUMBEROFBUTTONTYPES 3
#define COMMAND 2

enum direction_t{UP=1, DOWN=-1};
/*FIRST,SECOND,THIRD,FOURTH,BETWEEN_FIRST_SECOND,BETWEEN_SECOND_THIRD,BETWEEN_THIRD_FOURTH*/
enum floor_t{FIRST=0,SECOND=1,THIRD=2,FOURTH=3};

enum event_t{FLOOR_REACHED, NEW_ORDER, STOP_PRESSED};

bool noObstruction();
bool floorHaveOrder();
void handleStop();
void handleEmergencyStop();
void addOrderToList(buttonType,floor);
void checkSensor();




#endif //MAIN_H
