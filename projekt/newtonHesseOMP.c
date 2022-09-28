/**
 * @file main.c
 * @brief 
 * @version 0.1
 * @date 2022-09-20
 * 
 * @author Leon Oleschko
 * 
 */

#include "methods.h"
#include <omp.h>

//Methode mit Hessematrix
double Hessematrix[3][3];
double gradient[3];

void calcHesse();
double newtonstepMitHesse();

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
	err = newtonstepMitHesse(newtoniterationnumber);
	newtoniterationnumber++;
	
	}while(err > NEWTON_PRECISION);

	//trajectory(-0.009717 , 0.021646 , 0.000793, false);
	
	// save the trajectory
	double abserror = trajectory(&v_start, true);
	printf("\n optimal: v_start = {%.15f, %.15f, %.15f},      err = %g\n", v_start[0],v_start[1],v_start[2], abserror);	
	
	return(0);
}


void calcHesse()
{
	#pragma omp parallel for
	for(int j = 0 ; j < 3 ; j++ )
	{
		double v[3];
		v[0] = v_start[0];
		v[1] = v_start[1];
		v[2] = v_start[2];

		gradient[j] = 0;
		v[j]+=h;
		err = trajectory(&v, false);
		gradient[j] = err;

		for(int i = 0 ; i < 3 ; i++)
		{
			Hessematrix[j][i] -= err;
		}
		v[j]-=h;
		err = trajectory(&v, false);
		gradient[j] = err;
		gradient[j]/=h;
		for(int i = 0 ; i < 3 ; i++)
		{
			Hessematrix[j][i] += err;
			Hessematrix[j][i] /= h;
		}

	}


	#pragma omp parallel for
	for(int k = 0 ; k < 3 ; k++)
	{
		double v[3];
		v[0] = v_start[0];
		v[1] = v_start[1];
		v[2] = v_start[2];

		v[k]+=h;
		for(int i = 0 ; i < 3 ; i++)
		{
			v[i]+=h;
			err = trajectory(&v, false);
			Hessematrix[i][k] += err/h; 
			v[i]-=h;
			err = trajectory(&v, false);
			Hessematrix[i][k] -= err/h; 
			Hessematrix[i][k] /=h; 
		
		}
		v[k]-=h;
	}
	//err = trajectory(&v_start, false);
		
}

double newtonstepMitHesse(int ni)
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
	calcHesse();

		
  	double solution[3] = {0 , 0 , 0};

	//Gauss Algorithmus für Dreiecksform
	for(int i = 0 ; i < 3 ; i++)
	{   
		double fac1 = Hessematrix[i][i];
		if(Hessematrix[i][i] != 0)
		{
			for(int j = 0 ; j < 3 ; j++)
			{
				Hessematrix[i][j]/=fac1;
			}
			gradient[i]/=fac1;
		}
		for(int j = i+1 ; j < 3 ; j++)
		{
			double fac2 = Hessematrix[j][i];
			gradient[j]-=gradient[i]*fac2;
			for(int k = 0 ; k < 3 ; k++)
			{   
				Hessematrix[j][k]-=Hessematrix[i][k]*fac2;
			}
		}
	}

	//solve System 
        for(int i = 2 ; i > 0 ; i--)
        {
            for(int j = 0 ; j < i ; j++)
            {
                double fac = Hessematrix[j][i];                   
                Hessematrix[j][i]=0;
                gradient[j]-=gradient[i]*fac;
            }
        }
	double gain = 1;
    v_start[0]-=gain*gradient[0];
	v_start[1]-=gain*gradient[1];
	v_start[2]-=gain*gradient[2];

	//solve boundary value problem with new initial velocity
	double abserror = trajectory(&v_start, false);
	printf("%4d. iteration: v_start = {%.10f, %.10f, %.10f},      err = %g\n", ni,v_start[0],v_start[1],v_start[2], abserror);
    return(abserror);
}