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


#include "hd.h"

void fft(real_t real[FFT_SIZE], img_t img[FFT_SIZE],
		real_twid_t real_twid[FFT_SIZE / 2], img_twid_t img_twid[FFT_SIZE / 2]) {

	#pragma HLS INTERFACE s_axilite port=return bundle=FFT_PERIPH_BUS
	#pragma HLS INTERFACE s_axilite port=img bundle=FFT_PERIPH_BUS
	#pragma HLS INTERFACE s_axilite port=real bundle=FFT_PERIPH_BUS
	#pragma HLS INTERFACE s_axilite port=img_twid bundle=FFT_PERIPH_BUS
	#pragma HLS INTERFACE s_axilite port=real_twid bundle=FFT_PERIPH_BUS


	int even, odd, span, log, rootindex,i;
	double temp;
	log = 0;

	for (span = FFT_SIZE >> 1; span; span >>= 1, log++) {
		odd=span;
		for (i = 0; i < FFT_SIZE/2; i++) {

			odd |= span;
			even = odd ^ span;

			temp = real[even] + real[odd];
			real[odd] = real[even] - real[odd];
			real[even] = temp;

			temp = img[even] + img[odd];
			img[odd] = img[even] - img[odd];
			img[even] = temp;

			rootindex = (even << log) & (FFT_SIZE - 1);
			temp = real_twid[rootindex] * real[odd]
					- img_twid[rootindex] * img[odd];
			img[odd] = real_twid[rootindex] * img[odd]
					+ img_twid[rootindex] * real[odd];
			real[odd] = temp;
			odd++;
		}
	}

}
