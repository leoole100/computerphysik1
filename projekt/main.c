/**
 * @file main.c
 * @brief Runs the newton algorithm to find the optimal initial velocity
 * @version 0.1
 * @date 2022-09-20
 * 
 * @author Moritz Schroer
 * 
 */

#include "methods.h"

void makeJacobian();
double newtonstep(int newtoniterationnumber);

//Jacobian matrix of the errorfunction 
double Jacobian[3][3];
//counter of the newton steps
int newtoniterationnumber = 1;


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
	if(newtoniterationnumber > NEWTON_STEPS)
	{
		printf("\n error couldn't find optimal solution\n");
		exit(0);
	}
	err = newtonstep(newtoniterationnumber);
	newtoniterationnumber++;
	
	}while(err > NEWTON_PRECISION);

	// save the trajectory
	double abserror = trajectory(&v_start, true);
	printf("\n optimal: v_start = {%.15f, %.15f, %.15f},      err = %g\n", v_start[0],v_start[1],v_start[2], abserror);	
	
	return(0);
}


/**
 * @brief 
 * 
 * @author Moritz Schroer
 */
void calcJacobian()
{	
	/*
    vec(v_n)=v_start^(n)
	Computes Jacobian Matrix Derr(vec(v_n)) by calculating the partial dericatives with difference quotient:

	d err_i/d x = (errvec(v_start[0] + h * e_x , v_start[1] , v_start[2]) - errvec(v_start[0] , v_start[1] , v_start[2])) / h
	d err_i/d y = (errvec(v_start[0] , v_start[1] + h *e_y , v_start[2]) - errvec(v_start[0] , v_start[1] , v_start[2])) / h
	d err_i/d z = (errvec(v_start[0] , v_start[1] , v_start[2] + h *e_z) - errvec(v_start[0] , v_start[1] , v_start[2])) / h

	and then resets errvec = err(v_start[0] , v_start[1] , v_start[2])

	inputs:
		None

	outputs :
		None

	————————————————————————————————————————————————
	*/

	for(int j = 0 ; j < 3 ; j++ )
	{
		
		v_start[j]+=h;
		err = trajectory(&v_start, false);

		for(int i = 0 ;  i < 3 ; i++)
		{
			Jacobian[i][j]=errvec[i];
		}
		v_start[j]-=h;

		err = trajectory(&v_start, false);
		for(int i = 0 ; i < 3 ; i++)
		{
			Jacobian[i][j]-=errvec[i];
			Jacobian[i][j]/=h;
		}
		
	}
}

/**
 * @brief 
 * 
 * @author Moritz Schroer
 */
double newtonstep(int ni)
{
	/*
	v_start^(n) := (v_start[0] , v_start[1] , v_start[2])

	Calculates Newtonstep v_start^(n+1) = v_start^(n) - (Derr(v_start^(n)))^-1 * err(v_start^(n))	

	Therefore the following System of equations is solved with Jacobi iteration (deltav_n := (delta v_x , delta v_y , delta v_z)):

	Derr(v_start^(n)) * deltav_n    =     -err(v_start^(n)) 
	
	then set:

	v_start^(n+1) = v_start^(n) + deltav_n

	inputs:
		None
	outputs:
		None

	—————————————————————————————————————————————————-
	*/


    /* numerical solution of equation system (doesn't work)
    -----------------------------------------------------------------------------------------------------------
    //calculate Jacobian Matrix Derr of the errorfunction
	calcJacobian();

	//delta_s vector to solve the linear system of equation with Jacobi iteration method
	double deltav_n[3] = {-0. , -0.0 , -0.0};
	double deltav_np[3] = {0 , 0 , 0}; 
	//sles := Derr*deltas, if | sles- |^2 < 1e-7 break; because linear equation system is solved.
	
	double abserrles = 100;
	//initial guess of the solution of the linear system of equations
	do{	
		
			for(int j = 0 ; j < 3 ; j++)
			{
				printf("errvec[%d] = %lf\n", j, errvec[j]);
			
		}

		//do Jacobi step
		deltav_np[0] =  (-errvec[0]-Jacobian[0][1]*deltav_n[1]-Jacobian[0][2]*deltav_n[2])/Jacobian[0][0]; 
		deltav_np[1] =  (-errvec[1]-Jacobian[1][0]*deltav_n[0]-Jacobian[1][2]*deltav_n[2])/Jacobian[1][1]; 
		deltav_np[2] =  (-errvec[2]-Jacobian[2][0]*deltav_n[0]-Jacobian[2][1]*deltav_n[1])/Jacobian[2][2]; 
		deltav_n[0] = deltav_np[0];
		deltav_n[1] = deltav_np[1];
		deltav_n[2] = deltav_np[2];
		

        double sles[3] = {0.0 , 0.0 , 0.0};
		//calculate Derr(v_start^(n))*(s_x,s_y,s_z)
		for(int i = 0 ; i < 3 ; i++)
		{
			for(int j = 0 ; j < 3 ; j++)
			{
				sles[i]+=(Jacobian[i][j]*deltav_n[j]);
			}
		}
		abserrles = (sles[0]+errvec[0])*(sles[0]+errvec[0])+(sles[1]+errvec[1])*(sles[1]+errvec[1])+(sles[2]+errvec[2])*(sles[2]+errvec[2]);
		//check if abserrless is finte else exit programm and display error message
		if(isinf(abserrles) || isnan(abserrles))
		{
			printf("error couldnt perform Newtonstep abserror exceeds limits %lf\n",abserrles);
			exit(0);
		}
	}while(abserrles > powf(10,-8));
	----------------------------------------------------------------------------------------------------------------------------*/

        //calculate Jacobian Matrix Derr of the errorfunction
	calcJacobian();

		
  	double solution[3] = {0 , 0 , 0};

	//Gauss Algorithmus für Dreiecksform
	for(int i = 0 ; i < 3 ; i++)
	{   
		double fac1 = Jacobian[i][i];
		if(Jacobian[i][i] != 0)
		{
			for(int j = 0 ; j < 3 ; j++)
			{
				Jacobian[i][j]/=fac1;
			}
			errvec[i]/=fac1;
		}
		for(int j = i+1 ; j < 3 ; j++)
		{
			double fac2 = Jacobian[j][i];
			errvec[j]-=errvec[i]*fac2;
			for(int k = 0 ; k < 3 ; k++)
			{   
				Jacobian[j][k]-=Jacobian[i][k]*fac2;
			}
		}
	}

	//solve System 
	for(int i = 2 ; i > 0 ; i--)
	{
		for(int j = 0 ; j < i ; j++)
		{
			double fac = Jacobian[j][i];                   
			Jacobian[j][i]=0;
			errvec[j]-=errvec[i]*fac;
		}
	}

	double gain = .1;

	v_start[0]-=gain*errvec[0];
	v_start[1]-=gain*errvec[1];
	v_start[2]-=gain*errvec[2];

	//solve boundary value problem with new initial velocity
	double abserror = trajectory(&v_start, false);
	printf("%4d. iteration: v_start = {%.10f, %.10f, %.10f}, err = %e\n", ni,v_start[0],v_start[1],v_start[2], abserror);
    return(abserror);
}
