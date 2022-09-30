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
#define TMAX 2800 // for plots
#define SUB_STEPS 1e3
#define h 1e-12 //constant needed for calculation of difference qoutioent f'(x)=(f(x+h)-f(x))/h
#define G  1.488e-34 // in AU^3 / kg day^2
#define NEWTON_STEPS 1000
#define NEWTON_PRECISION 1e-5
//#define  double m =  367.0; // in kg // kuertzt sich eingenlicht raus


/*initial Values and boundaryconditions:
-----------------------------------------
*/

//boundary conditions of the start:
const double r_start[3]={0.9018647801517535, 0.4249622724991888, 0.007482330796928186}; // at day 10
//double r_end[3] = {0.399419613380915, -0.511606579819174, -1.66388661871306}; // at day 519+10

//initial guess of start velocity, parameter that needs to be optimized
//double v_start[3] ={-0.00937424, 0.0215741, 0.000878397}; // historic data at day 10
//double v_start[3] = {-0.0094588, 0.0217687, 0.000799341}; //manually found value for swing by
//double v_start[3] = {-0.009457376603971, 0.021741396850579, 0.000814226449307}; // best currently known guess
double v_start[3] = {-0.00944, 0.021735, 0.000818};

//boundary conditions for the error function
/*#define NUM_ENDS 4
const int end_days[NUM_ENDS] = {800, 1200, 1000, 2400};
const double r_end[NUM_ENDS][3] = {{-4.62084, 1.63723, -1.33561}, {-2.60042, 0.545625, -2.56051}, {-3.8573, 1.16459, -2.08768}, {-4.30592, 2.11504, 1.58378}};
const double v_end[NUM_ENDS][3] = {{0.00285374, -0.00191824, -0.00460522}, {0.00761086, -0.00301413, -0.00115772}, {0.00479005, -0.00217984, -0.00353774}, {-0.00356086, 0.000815535, -0.00439962}};*/
#define NUM_ENDS 3
const int end_days[NUM_ENDS] = {800, 1200, 2400};
const double r_end[NUM_ENDS][3] = {{-4.62347, 1.63907, -1.33185}, {-2.60794, 0.549103, -2.55933}, {-4.30138, 2.11468, 1.58702}};
const double v_end[NUM_ENDS][3] = {{0.00273899, -0.00187134, -0.00418154}, {0.00776259, -0.00362208,-0.00148691}, {-0.00415754, 0.000517048, -0.00357939}};

