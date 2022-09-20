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
#define TMAX 800 // in days
#define SUB_STEPS 1e3
#define h 0.0000001 //constant needed for calculation of difference qoutioent f'(x)=(f(x+h)-f(x))/h
#define G  1.488e-34 // in AU^3 / kg day^2
#define maxnewtonsteps 25
//#define  double m =  367.0; // in kg // kuertzt sich eingenlicht raus


/*initial Values and boundaryconditions:
-----------------------------------------
*/

//boundary conditions of the location:
double r_start[3]={0.892490713706592,0.446536028000082,0.008360720388102};//at day10
double r_end[3] = {-4.62083867637899, 1.63724477946744 , -1.33560490377459};//at day 810

//boundary condition of the velocity: 
double v_end[3] = {0.002853729 , -0.00191825 , -0.00460521};//at day 800

//initial guess of velocity, size that needs to be optimized
//double v_start[3] = {-0.002976 , 0.020794 , -0.007324}; //at day 10
double v_start[3] = {-0.0094588, 0.0217687, 0.000799341}; //manually found value for swing by

