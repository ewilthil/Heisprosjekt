#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include "elev.h"

int main() {
	clock_t start, stop;
	double t = 0.0;
	int flag=0;
	while(1){
		if(elev_get_stop_signal){
			start = clock ();
			flag=1;
		}
		if(!(elev_get_stop_signal)&&flag){
			stop=clock ();
			break;
		}
	}
	start = clock();
	while((stop - start)/CLOCKS_PER_SEC < 3){
		checkbuttons();
		stop = clock();
	}

t = (double) (stop-start)/CLOCKS_PER_SEC*1000;

printf("Run time: %f ms\n", t);

return(0);
} /* main */
