/*******************************************************************************
 Source: MachSuite (please look the LICENSE below for details)
 Modified by: David Berrazueta & Byron Navas
 Associated Filenames: fcn.cpp, hd.h, tb.cpp, gold.dat, directives.tcl
 Purpose: FFT Vivado HLS version
 Device:
 Clock Period: 10ns
 Clock Uncertainty: default (12.5%)
 Revision History: November 11, 2019
 *******************************************************************************
 FFT IP-core v0.1
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

#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include "hd.h"

int main() {

	FILE *fp;

	real_t real[FFT_SIZE];
	img_t img[FFT_SIZE];
	real_twid_t real_twid[FFT_SIZE / 2];
	img_twid_t img_twid[FFT_SIZE / 2];


	//set up twiddles...
	double typed;
	int n, N;
	N = FFT_SIZE;

	//Pre-calc twiddles
	for (n = 0; n < (N >> 1); n++) {
		typed = (double) (twoPI * n / N);
		real_twid[n] = cos(typed);
		img_twid[n] = (-1.0) * sin(typed);
	}

	//Init data
	for (int i = 0; i < FFT_SIZE; i++) {
		real[i] = (double) (i);
		img[i] = (double) (i);
	}

	// Run the FFT main function
	fft(real, img, real_twid, img_twid);

	// Save function results as .dat
	fp = fopen("results.dat", "w");
	for (int i = 0; i < FFT_SIZE; i++) {
		fprintf(fp, "%.2f : %.2f\n", real[i], img[i]);
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
