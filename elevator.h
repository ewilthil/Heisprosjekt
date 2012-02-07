/*
Spørsmål eller kommentarer?
*/
#ifndef MAIN_H
#define MAIN_H

#define NUMBEROFFLOORS 4 
#define NUMBEROFBUTTONTYPES 3
#define COMMAND 2

enum direction_t{ UP, DOWN};

enum floor_t{1,2,3,4,1.5,2.5,3.5};

enum event_t{FLOOR_REACHED, NEW_ORDER, STOP_PRESSED};

bool noObstruction();
bool floorHaveOrder();
void handleStop();
void handleEmergencyStop();
void addOrderToList(buttonType,floor);
void checkSensor();




#endif //MAIN_H
