/**
 * @file setup.h
 * @brief extra file to initialize initial values and constants  
 * @version 0.1
 * @date 2022-09-20
 *  
 */


/*Constants needed for the calculations:
------------------------------------------
*/
#define PLANET_PATH "data/planets/"
//#define TMAX 519 // in days // historic travel time
#define TMAX 2500  // at least 1269 for second check point
#define SUB_STEPS 1e3
#define h 1e-10 //constant needed for calculation of difference qoutioent f'(x)=(f(x+h)-f(x))/h
#define G  1.488e-34 // in AU^3 / kg day^2
#define maxnewtonsteps 1000
//#define  double m =  367.0; // in kg // kuertzt sich eingenlicht raus


/*initial Values and boundaryconditions:
-----------------------------------------
*/

//boundary conditions of the location:
double r_start[3]={0.9018647801517535, 0.4249622724991888, 0.007482330796928186}; // at day 10
double r_end[3] = {-4.99443408522095, 2.0463303553843777, -0.08102002848437442}; // at day 519+10

// end point array
#define CHECKPOINTS 3
const double r_checkpoints[3][3]= {{-4.99443, 2.04633, -0.08102}, {-2.12764, 0.340793, -2.60281},
	{-1.71071, 1.26679, 2.68188} // bei 1966 eingentlich unnötig

};

//boundary condition of the velocity: 
double v_end[3] = {-0.00028974465364584034, -0.000899297755505124, -0.004710081570797581}; // at day 519+10

//initial guess of velocity, size that needs to be optimized
//double v_start[3] ={-0.00937424, 0.0215741, 0.000878397}; // historic data at day 10
//double v_start[3] = {-0.0094588, 0.0217687, 0.000799341}; //manually found value for swing by
double v_start[3] =  {-0.0094042634 , 0.0217337274 , 0.0008145859}; // found with grid search
/*
{-0.0096,		0.0218,	0.0008} // schießt zu früh vorbei
{-0.00936001, 	0.02171,0.000830002} zu spät
{-0.0094419, 0.0217498, 0.000818747} zu spät
 {-0.00946865, 0.0217569, 0.000808898} zu spät
 {-0.00940468, 0.0217319, 0.0008199}

 {-0.009404  ,  0.021736  ,  0.000813}
*/