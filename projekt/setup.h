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
#define TMAX 519 // in days // historic travel time
//#define TMAX 1900 // for plots
#define SUB_STEPS 1e1
#define h 0.0000001 //constant needed for calculation of difference qoutioent f'(x)=(f(x+h)-f(x))/h
#define G  1.488e-34 // in AU^3 / kg day^2
#define maxnewtonsteps 25
//#define  double m =  367.0; // in kg // kuertzt sich eingenlicht raus


/*initial Values and boundaryconditions:
-----------------------------------------
*/

//boundary conditions of the location:
double r_start[3]={0.9018647801517535, 0.4249622724991888, 0.007482330796928186}; // at day 10
double r_end[3] = {-4.99443408522095, 2.0463303553843777, -0.08102002848437442}; // at day 519+10

//boundary condition of the velocity: 
double v_end[3] = {-0.00028974465364584034, -0.000899297755505124, -0.004710081570797581}; // at day 519+10

//initial guess of velocity, size that needs to be optimized
//double v_start[3] ={-0.00937424, 0.0215741, 0.000878397}; // historic data at day 10
//double v_start[3] = {-0.0094588, 0.0217687, 0.000799341}; //manually found value for swing by
double v_start[3] = {-0.00946865, 0.0217569, 0.000808898}; // found with grid search
/*
{-0.0096,0.0218,0.0008} // schießt zu früh vorbei
{-0.00936001, 0.02171, 0.000830002} zu spät
{-0.0094419, 0.0217498, 0.000818747} zu spät
*/
