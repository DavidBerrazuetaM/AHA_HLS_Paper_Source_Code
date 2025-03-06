/*******************************************************************************
 Source: MachSuite (please look the LICENSE below for details)
 Modified by: David Berrazueta & Byron Navas
 Associated Filenames: fft_test.cpp, fft.cpp, fft.h
 Purpose: FFT software version for BSP Standalone (ARM Cortex A9)
 Revision History: November 11, 2019
 *******************************************************************************
 FFT v0.1
 *******************************************************************************
 MachSuite (BSD-3) License

Copyright (c) 2014-2015, the President and Fellows of Harvard College.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of Harvard University nor the names of its contributors may
  be used to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include "fft.h"
#include <math.h>
#include "xil_cache.h"
#include "xil_cache_l.h"

#include "xtime_l.h"
#define TIMEDIFF(t1,t2)  (t2 - t1)
#define MICROSECONDS(t) (1000000.0 * t / COUNTS_PER_SECOND)
XTime start, end;
double acumulador = 0;


void startTest() {
	XTime_GetTime(&start);
}


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
	double data_x[FFT_SIZE];
	double data_y[FFT_SIZE];
	double img[FFT_SIZE / 2];
	double real[FFT_SIZE / 2];
	int i;

	//set up twiddles...
	double typed;
	int n, N;
	N = FFT_SIZE;


	//Pre-calc twiddles
	for (n = 0; n < (N >> 1); n++) {
		typed = (double) (twoPI * n / N);
		real[n] = cos(typed);
		img[n] = (-1.0) * sin(typed);
	}

	//Init data
	for (i = 0; i < FFT_SIZE; i++) {
		data_x[i] = (double) (i);
		data_y[i] = (double) (i);
	}


	int g;
			int pruebas = 1000;
			double acu_aux;
			for (g = 0; g < pruebas; g++) {
	startTest();
	fft(data_x, data_y, real, img);
	acu_aux = endTest();
			}
			printf("Run-time = %.2f usec...\n", acu_aux / pruebas);

	printf("%.2f\n",data_x[0]);
	printf("%.2f\n",data_x[1023]);

	return 0;
}
