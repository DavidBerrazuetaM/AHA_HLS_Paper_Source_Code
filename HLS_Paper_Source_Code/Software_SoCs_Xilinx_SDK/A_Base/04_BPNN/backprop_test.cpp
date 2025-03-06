/*******************************************************************************
 Source: MachSuite (please look the LICENSE below for details)
 Modified by: David Berrazueta & Byron Navas
 Associated Filenames: backprop_test.cpp, backprop.cpp, backprop.h
 Purpose: BPNN software version for BSP Standalone (ARM Cortex A9)
 Revision History: November 11, 2019
 *******************************************************************************
 BPNN v0.1
 *******************************************************************************
 BSD-3 License (backprop implementation)

Copyright (c)2008-2011 University of Virginia
Copyright (c) 2014-2015, the President and Fellows of Harvard College
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted without royalty fees or other restrictions, provided that the
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.
* Neither the name of the University of Virginia, the Dept. of Computer Science,
  nor the names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY OF VIRGINIA OR THE SOFTWARE AUTHORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/
#include "backprop.h"
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

	TYPE weights1[input_dimension * nodes_per_layer], weights2[nodes_per_layer
			* nodes_per_layer], weights3[nodes_per_layer * possible_outputs],
			biases1[nodes_per_layer], biases2[nodes_per_layer],
			biases3[possible_outputs], training_data[training_sets
					* input_dimension], training_targets[training_sets
					* possible_outputs];
	int g;
	   			int pruebas = 10;
	   			double acu_aux;
	   			for (g = 0; g < pruebas; g++) {
	startTest();
	backprop(weights1, weights2, weights3, biases1, biases2, biases3,
			training_data, training_targets);
	acu_aux = endTest();
	       			}
	       			printf("Run-time = %.2f usec...\n", acu_aux / pruebas);


}
