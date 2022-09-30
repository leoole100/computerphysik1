/**
 * @file leapFrog.c
 * @brief simulation of the trajectory with the leap frog algorithm
 * @version 1.0
 * @date 2022-09-08
 * 
 * @author Leon Oleschko
 * 
 */

#include "methods.h"

int main(){

	/*v_start[0] *= 1.03;
	v_start[1] *= 1.004;
	v_start[2] *= 0.92;*/


	double err = trajectory(&v_start, true);	
	printf("the error: %g\n",err);

	return 0;
}
