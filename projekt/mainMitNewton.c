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
	
	}while(err > powf(10,-12));

	//trajectory(-0.009717 , 0.021646 , 0.000793, false);
	
	printf("\n found optimal values of v_start : [%lf  ,  %lf  ,  %lf]\n", v_start[0], v_start[1] , v_start[2]);
	printf("err = %lf\n",sqrt(err));
	
	
	return(0);
}
