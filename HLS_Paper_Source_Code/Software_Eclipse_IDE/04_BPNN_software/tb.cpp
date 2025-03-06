/*******************************************************************************
 Source: MachSuite (please look the LICENSE below for details)
 Modified by: David Berrazueta & Byron Navas
 Associated Filenames: fcn.cpp, hd.h, tb.cpp, gold.dat
 Purpose: BPNN software version
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

#include <stdio.h>
#include <cstdlib>
#include "hd.h"

int main() {

	FILE *fp;

	TYPE weights1[input_dimension * nodes_per_layer], weights2[nodes_per_layer
			* nodes_per_layer], weights3[nodes_per_layer * possible_outputs],
			biases1[nodes_per_layer], biases2[nodes_per_layer],
			biases3[possible_outputs], training_data[training_sets
					* input_dimension], training_targets[training_sets
					* possible_outputs];

	for (int j = 0; j < input_dimension * nodes_per_layer; j++) {
		weights1[j] = 0.3;
	}
	for (int j = 0; j < nodes_per_layer * nodes_per_layer; j++) {
		weights2[j] = 0.3;
	}
	for (int j = 0; j < nodes_per_layer * possible_outputs; j++) {
		weights3[j] = 0.3;
	}
	for (int j = 0; j < nodes_per_layer; j++) {
		biases1[j] = 0.3;
	}
	for (int j = 0; j < nodes_per_layer; j++) {
		biases2[j] = 0.3;
	}
	for (int j = 0; j < possible_outputs; j++) {
		biases3[j] = 0.3;
	}
	for (int j = 0; j < training_sets * input_dimension; j++) {
		training_data[j] = j;
	}
	for (int j = 0; j < training_sets * possible_outputs; j++) {
		training_targets[j] = j;
	}

	// Run the BPNN main function
	backprop(weights1, weights2, weights3, biases1, biases2, biases3,
					training_data, training_targets);

	// Save function results as .dat
	fp = fopen("results.dat", "w");

	for (int i = 0; i < input_dimension * nodes_per_layer; i++) {
		fprintf(fp, "%.3f\n", double(weights1[i]));
	}
	for (int i = 0; i < nodes_per_layer * nodes_per_layer; i++) {
		fprintf(fp, "%.3f\n", double(weights2[i]));
	}
	for (int i = 0; i < nodes_per_layer * possible_outputs; i++) {
		fprintf(fp, "%.3f\n", double(weights3[i]));
	}
	for (int i = 0; i < nodes_per_layer; i++) {
		fprintf(fp, "%.3f\n", double(biases1[i]));
	}
	for (int i = 0; i < nodes_per_layer; i++) {
		fprintf(fp, "%.3f\n", double(biases2[i]));
	}
	for (int i = 0; i < possible_outputs; i++) {
		fprintf(fp, "%.3f\n", double(biases3[i]));
	}
	for (int i = 0; i < training_sets * input_dimension; i++) {
		fprintf(fp, "%.3f\n", double(training_data[i]));
	}
	for (int i = 0; i < training_sets * possible_outputs; i++) {
		fprintf(fp, "%.3f\n", double(training_targets[i]));
	}
	fclose(fp);

	// Check function result against golden results
	printf("Comparing against output data \n");
	if (system("diff -w results.dat gold.dat")) {

		fprintf(stdout, "**********************************************\n");
		fprintf(stdout, "FAIL: Output DOES NOT match the golden results\n");
		fprintf(stdout, "**********************************************\n");
		return 1;
	} else {
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "PASS: The output matches the golden results!\n");
		fprintf(stdout, "*******************************************\n");
		return 0;
	}

}
