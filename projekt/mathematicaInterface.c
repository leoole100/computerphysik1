/**
 * @file mathematicaInterface.c
 * @brief Interface for the mathematica Notebook to the c functions in the method file
 * @version 0.1
 * @date 2022-09-20
 * 
 * @author Leon Oleschko
 * 
 */

#include "WolframLibrary.h"
#include "methods.h"

/**
 * @brief runs the trajectory from x_start with v_start as argument
 * 
 * @param libData 
 * @param Argc 
 * @param Args v_start_x, v_start_y, v_start_z
 * @param Res 
 * @return DLLEXPORT 
 */
DLLEXPORT int mathematicaTrajectoryError(WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res) {
	mreal v_start_param[3];
	mreal result;
	v_start[0] = MArgument_getReal(Args[0]);
	v_start[1] = MArgument_getReal(Args[1]);
	v_start[2] = MArgument_getReal(Args[2]);

	result = trajectory(&v_start, false);
	
	MArgument_setReal(Res,result);
	return LIBRARY_NO_ERROR;
}


/**
 * @brief returns the evaluated error function
 * 
 * @param libData 
 * @param Argc 
 * @param Args r_x, r_y, r_z, v_x, v_y, v_z
 * @param Res 
 * @return DLLEXPORT 
 */
DLLEXPORT int mathematicaErrorFunction(WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res) {
	mreal v_start_param[3];
	mreal result;

	double r[3], v[3];

	r[0] = MArgument_getReal(Args[0]);
	r[1] = MArgument_getReal(Args[1]);
	r[2] = MArgument_getReal(Args[2]);
	v[0] = MArgument_getReal(Args[3]);
	v[1] = MArgument_getReal(Args[4]);
	v[2] = MArgument_getReal(Args[5]);
	
	result = errfunction(&r, &v);

	MArgument_setReal(Res,result);
	return LIBRARY_NO_ERROR;
}