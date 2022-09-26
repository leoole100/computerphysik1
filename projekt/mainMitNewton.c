/**
 * @file main.c
 * @brief 
 * @version 0.1
 * @date 2022-09-20
 * 
 * @author Moritz Schroer
 * 
 */

#include "methodsMitNewton.h"

int main()
	{
		
	// loop    /////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("begin main\n");
	
	
	__uint32_t newtoniterationnumber = 1;
	
	//1. Methode: shootingmethode mit Newton-Raphson Verfahren
	double err = 100;
	printf("begin Newtoniteration\n");
	do{
	//if maximum number of newtonsteps is reached exit program and display error message
	if(newtoniterationnumber > maxnewtonsteps)
	{
		printf("\n error couldn't find optimal solution\n");
		exit(0);
	}
	err = newtonstep(newtoniterationnumber);
	newtoniterationnumber++;
	
	}while(err > 1e-12);

	//trajectory(-0.009717 , 0.021646 , 0.000793, false);
	
	// save the trajectory
	double abserror = trajectory(&v_start, true);
	printf("\n optimal: v_start = {%.15f, %.15f, %.15f},      err = %g\n", v_start[0],v_start[1],v_start[2], abserror);	
	
	return(0);
}
