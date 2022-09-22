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
#define TMAX 2410 // for plots
#define SUB_STEPS 1e3
#define h 1e-7 //constant needed for calculation of difference qoutioent f'(x)=(f(x+h)-f(x))/h
#define G  1.488e-34 // in AU^3 / kg day^2
#define maxnewtonsteps 50
//#define  double m =  367.0; // in kg // kuertzt sich eingenlicht raus


/*initial Values and boundaryconditions:
-----------------------------------------
*/

//boundary conditions of the location:
double r_start[3]={0.9018647801517535, 0.4249622724991888, 0.007482330796928186}; // at day 10
//double r_end[3] = {0.399419613380915, -0.511606579819174, -1.66388661871306}; // at day 519+10

//boundary condition of the velocity: 
//double v_end[3] = {0.0111337032, -0.002321440029, 0.010960436}; // at day 519+10


//initial guess of velocity, size that needs to be optimized
//double v_start[3] ={-0.00937424, 0.0215741, 0.000878397}; // historic data at day 10
//double v_start[3] = {-0.0094588, 0.0217687, 0.000799341}; //manually found value for swing by
double v_start[3] = {-0.00946868, 0.0217469, 0.0008089}; // found with grid search
/*
{-0.0096,		0.0218,	0.0008} // schießt zu früh vorbei
{-0.00936001, 	0.02171,0.000830002} zu spät
{-0.0094419, 0.0217498, 0.000818747} zu spät
 {-0.00946865, 0.0217569, 0.000808898} zu spät
*/

double errvec[3];

//boundary conditions for three days ({{x,y,z} , {x,y,z} , {x,y,z}} and {{v_x,v_y,v_z} , {v_x,v_y,v_z} , {v_x,v_y,v_z}})
double r_end[3][3] = {{-4.62083867637899,1.63724477946744,-1.33560490377459},{-2.60042145648637,0.545631844975302,-2.56050661752594},{-4.30591540663681,2.11505251044,1.58377693788273}};
double v_end[3][3] = {{-0.002853729281941,0.00191824512039,0.00460521200781},{0.00761085,-0.0030141465,-0.00115772},{-0.00356085,0.000815542,-0.00439961}};

//save calculated values of the three days, on which the boundaryconditions are given 
double errpoints[3][3];
double errvelocitys[3][3];

//Number of the three days on which the boundary conditions are given 
int errdays[3] = {800, 1200, 2400};
//index (errday[0]=800 ,  errday[errdaycounter+1] = errday[1] = 1200 , ...)
int errdaycounter = 0;