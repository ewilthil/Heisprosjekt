#include "avlusing.h"

void debug_printDestinationMatrix(){
	int i,k;
	printf("Floors:\t 1 \t 2 \t 3 \t 4 \t\n");
	for(i=0;i<NUMBEROFBUTTONTYPES;i++){
		if(i==0)
			printf("UP\t");
		else if(i==1)
			printf("DOWN\t");
		else if(i==2)
			printf("CMND\t");
		else
			printf("Ukjent etasje. Erik har driti seg ut. Sjekk funksjonen..\n");
		for(k=0;k<NUMBEROFFLOORS;k++){
			printf("%d\t",destinationMatrix[i][k]);
		}
		printf("\n");
	}
}
void debug_testDestinationMatrix(){
	ctrl_addOrderToList(UP,FIRST);
	ctrl_addOrderToList(DOWN,SECOND);
	ctrl_addOrderToList(COMMAND,THIRD);
	ctrl_addOrderToList(UP,FOURTH);
	printf("Fasit:\n");
	printf("Floors:\t 1 \t 2 \t 3 \t 4 \t\n");
	printf("UP\t 	  1 \t 0 \t 0 \t 1 \t\n");
	printf("DOWN\t	  0 \t 1 \t 0 \t 0 \t\n");
	printf("CMND\t	  0 \t \ \t 1 \t 0 \t\n");
	printf("Vårt resultat:\n");
	debug_printDestinationMatrix();
}
