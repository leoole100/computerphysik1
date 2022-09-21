/**
 * @file main.c
 * @brief 
 * @version 0.1
 * @date 2022-09-20
 * 
 * @author Moritz Schroer
 * 
 */

#include "methods.h"

int main()
	{
		
	// loop    /////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("begin main\n");
	
	__uint32_t newtoniterationnumber = 1;
	
	//1. Methode: shootingmethode mit Newton-Raphson Verfahren
	
	printf("begin Newtoniteration\n");
	do{
	//if maximum number of newtonsteps is reached exit program and display error message
	if(newtoniterationnumber > maxnewtonsteps)
	{
		printf("\n error couldn't find optimal solution\n");
		exit(0);
	}
	newtonstep(newtoniterationnumber);
	newtoniterationnumber++;
	//calculate new trajectory with the new v_start values from the newtonstep
	err = trajectory(&v_start, false);
	}while(err > powf(10,-28));

	//trajectory(-0.009717 , 0.021646 , 0.000793, false);
	
	printf("\n found optimal values of v_start : {%lf  ,  %lf  ,  %lf}\n", v_start[0], v_start[1] , v_start[2]);
	printf("err = %lf\n",sqrt(err));
    




	// 2. Methode: gitter von verschiedenen Anfangsgescshwindigkeiten absuchen in den intervallen
	//[v_start[i] - dv[i] * stepmins[i]   ,   v_start[i] + dv[i] * stepsplus[i]] wobei stepsminus , stepsplus
	//ganze Zahlen sind und dv[i] die schrittweite des Gitters für die Geschwindigkeitskomponenten 
	//dv[0] = dv_x, dv[1] = dv_y, dv[2] = dv_z ist
	/*
	printf("begin Gitter absuchen\n");
	double dv[3] = {0.000001 , 0.000001 , 0.000001}; //stepsize
	double stepsplus[3] = {50 , 50 , 25};
	double stepsminus[3] = {50 , 50 , 25};
	double minimum[4] = {100,0,0,0};

	v_start[0]-=stepsminus[0]*dv[0];
	v_start[1]-=stepsminus[1]*dv[1];
	v_start[2]-=stepsminus[2]*dv[2];

	for(int i = 0 ; i < stepsminus[0] + stepsplus[0] ; i++)
	{	
		v_start[0]+=dv[0];

		for(int j = 0 ; j < stepsminus[1] + stepsplus[1] ; j++)
		{
			v_start[1]+=dv[1];
			for(int k = 0 ; k < stepsminus[2] + stepsplus[2] ; k++)
			{
				v_start[2]+=dv[2];
				trajectory(v_start[0],v_start[1],v_start[2], false);
				double currenterr = errfunction();
				if(currenterr < minimum[0])
				{
					minimum[0] = currenterr;
					minimum[1] = v_start[0];
					minimum[2] = v_start[1];
					minimum[3] = v_start[2];
				}
				if(currenterr < powf(10,-14))
				{
					printf("found optimal solution err = %lf, v_start = [%lf  ,  %lf  ,  %lf]",currenterr,v_start[0],v_start[1],v_start[2]);
				}
			}
			v_start[2]-=(stepsminus[2] + stepsplus[2])*dv[2];
			printf("x-step: %d, y-step: %d, minimum = %lfe-10, v_start = [%lf  ,  %lf  ,  %lf]\n",i,j,minimum[0]*powf(10,10),minimum[1],minimum[2],minimum[3]);
		}
		v_start[1]-=(stepsminus[1] + stepsplus[1])*dv[1];
	}
	printf("minimum error at v = [%lf  ,  %lf ,  %lf]", minimum[1],minimum[2],minimum[3]);
	*/




	// 3. Methode: komponentenweise Minimum suchen
	/*
	double maxsteps[3] = {100, 100, 100};
	double dv[3] = {0.00001 , 0.00001 , 0.00001};
	v_start[0]-=dv[0]*maxsteps[0];
	v_start[1]-=dv[1]*maxsteps[1];
	v_start[2]-=dv[2]*maxsteps[2];
	for(int i = 0 ; i < 3 ; i++)
	{	
		trajectory(v_start[0],v_start[1],v_start[2], false);
		double currenterr = errfunction(); 
		double newcurrenterr = errfunction();
		double stepcounter = 0;
		
		do{
			if(stepcounter >= maxsteps[i])
			{
				printf("couldnt optimize v[%lf]\n",i);
				exit(0);
			}
			currenterr = newcurrenterr;
			v_start[i]+=dv[i];	
			trajectory(v_start[0],v_start[1],v_start[2], false);
			newcurrenterr = errfunction(); 
			stepcounter++;
		}while(newcurrenterr <= currenterr);
		printf("v_start[%d] optimized\n", i);
	}
	printf("optimal values: v_start = [%lf  ,  %lf  ,  %lf]\n", v_start[0],v_start[1],v_start[2]);
	*/
	
	return(0);
}
