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
#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include <iostream>
using namespace std;

#define FFT_SIZE 1024
#define twoPI 6.28318530717959

void fft(double real[FFT_SIZE], double img[FFT_SIZE],
		double real_twid[FFT_SIZE / 2], double img_twid[FFT_SIZE / 2]);

////////////////////////////////////////////////////////////////////////////////
// Test harness interface code.

struct bench_args_t {
	double real[FFT_SIZE];
	double img[FFT_SIZE];
	double real_twid[FFT_SIZE / 2];
	double img_twid[FFT_SIZE / 2];
};
