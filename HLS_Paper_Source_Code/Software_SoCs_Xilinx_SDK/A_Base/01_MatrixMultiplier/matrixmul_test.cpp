/*******************************************************************************
 Source: Custom coded
 Design by: David Berrazueta & Byron Navas
 Associated Filenames: matrixmul_test.cpp, matrixmul.cpp, matrixmul.h
 Purpose: Matrix Multiplier software version for BSP Standalone (ARM Cortex A9)
 Revision History: November 11, 2019
 *******************************************************************************
 Matrix Multiplier v0.1
 *******************************************************************************/
#include <stdio.h>
#include "xparameters.h"
#include <iostream>
#include "matrixmul.h"
using namespace std;
#include "xil_cache.h"
#include "xil_cache_l.h"

#include "xtime_l.h"
#define TIMEDIFF(t1,t2)  (t2 - t1)
#define MICROSECONDS(t) (1000000.0 * t / COUNTS_PER_SECOND)
XTime start, end;

void startTest() {
	XTime_GetTime(&start);
}

double acumulador = 0;

double endTest() {
	XTime_GetTime(&end);
	double time_curr = TIMEDIFF(start, end);
	double usec = MICROSECONDS(time_curr);
	acumulador = acumulador + usec;
	return acumulador;
}

int main() {
	Xil_DCacheDisable();//Disable the data cache.
	Xil_ICacheDisable();//Disable the instruction caches.
	Xil_L1ICacheDisable();//Disable the L1 cache
	Xil_L1DCacheDisable();//Disable the level 1 data cache.
	Xil_L2CacheDisable();//Disable the L2 cache.

	mat_a_t in_mat_a[2][2] = { { 0, 0 }, { 0, 1 } };
	mat_b_t in_mat_b[2][2] = { { 1, 1 }, { 0, 1 } };
	result_t hw_result[2][2];

	int g;
	int pruebas = 1000;
	double acu_aux;
	for (g = 0; g < pruebas; g++) {
		startTest();
		matrixmul(in_mat_a, in_mat_b, hw_result);
		acu_aux = endTest();
	}
	printf("Run-time = %.2f usec...\n", acu_aux/pruebas );


	cout << hw_result[0][0] << endl;
	cout << hw_result[0][1] << endl;
	cout << hw_result[1][0] << endl;
	cout << hw_result[1][1] << endl;

	return 0;
}
