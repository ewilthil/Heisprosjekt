#ifndef ELEVATOR_CTRL_PRIVATE_H
#define ELEVATOR_CTRL_PRIVATE_H
/*Støttefunksjoner for handlinger og betingelser*/
static void ctrl_setNewDirection();
static void ctrl_clearAllOrders();
static void ctrl_removeOrdersFromCurrentFloor();
static void ctrl_removeSingleOrder(buttonType_t);
static int ctrl_orderAtCurrentFloor();
static int ctrl_lowerFloorsHaveOrders();
static int ctrl_upperFloorsHaveOrders();

#endif
