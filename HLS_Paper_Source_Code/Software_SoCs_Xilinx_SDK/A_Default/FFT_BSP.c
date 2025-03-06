/*******************************************************************************
 Source: MachSuite (please look the LICENSE below for details)
 Modified by: David Berrazueta & Byron Navas
 Associated Filenames: FFT_BSP.c
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
#include "platform.h"
// Add BSP header files
#include <stdlib.h>  // Standard C functions, e.g. exit()#include <stdbool.h> // Provides a Boolean data type for ANSI/ISO-C#include "xparameters.h" // Parameter definitions for processor periperals#include "xscugic.h"     // Processor interrupt controller device driver#include "xfft.h"   // Device driver for HLS HW block#include "xbasic_types.h"
#include <math.h>
#include "xil_cache.h"
#include "xil_cache_l.h"

// HLS macc HW instance
XFft HlsFFT;
//Interrupt Controller Instance
XScuGic ScuGic;
// Global variable definitions - used by ISR
volatile static int RunHlsFFT = 0;
volatile static int ResultAvailHlsFFT = 0;

// Setup and helper functions
int setup_interrupt();
int hls_FFT_init(XFft *hls_FFTPtr);
void hls_FFT_start(void *InstancePtr);
// The ISR prototype
void hls_FFT_isr(void *InstancePtr);
// Software model of HLS hardware

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
	print("Program to test communication with HLS FFT block in PL\n\r");

	int status;
	int BaseAddress_real, HighAddress_real, TotalBytes_real, BitWidth_real,
			Depth_real;
	int length = 2;
	float twoPI = 6.28318530717959;
	float typed;
	int n;

	union {
		Xuint32 u32;
		Xfloat32 f32;
		Xuint8 u8[4];
		Xfloat64 f64;
	} unTemp;

	Xfloat64 A = 0;
	unTemp.f64 = A;

	status = hls_FFT_init(&HlsFFT);
	if (status != XST_SUCCESS) {
		print("HLS peripheral setup failed\n\r");
		exit(-1);
	}

	int i;
	for (i = 0; i < 1024; i++) {
		Xfloat64 A = (double) (i);
		unTemp.f64 = A;
		XFft_Write_real_r_Words(&HlsFFT, i * 2, &unTemp.f64, length);
		XFft_Write_img_Words(&HlsFFT, i * 2, &unTemp.f64, length);
	}

	for (i = 0; i < 1024; i++) {
		XFft_Read_real_r_Words(&HlsFFT, i * 2, &unTemp.f64, length);
		printf("real: %.2f\n", unTemp.f64);
		XFft_Read_img_Words(&HlsFFT, i * 2, &unTemp.f64, length);
		printf("img : %.2f\n", unTemp.f64);
	}

	for (i = 0; i < 512; i++) {
		typed = (double) (twoPI * i / 1024);
		Xfloat64 A = (double) (cos(typed));
		unTemp.f64 = A;
		XFft_Write_real_twid_Words(&HlsFFT, i * 2, &unTemp.f64, length);
		A = (double) ((-1.0) * sin(typed));
		unTemp.f64 = A;
		XFft_Write_img_twid_Words(&HlsFFT, i * 2, &unTemp.f64, length);
	}

//	for (i = 0; i < 512; i++) {
//		XFft_Read_real_twid_Words(&HlsFFT, i * 2, &unTemp.f64, length);
//		printf("real_twid :%.2f\n", unTemp.f64);
//		XFft_Read_img_twid_Words(&HlsFFT, i * 2, &unTemp.f64, length);
//		printf("img_twid :%.2f\n", unTemp.f64);
//	}

	if (XFft_IsReady(&HlsFFT))
		print("HLS peripheral is ready.  Starting... ");
	else {
		print("!!! HLS peripheral is not ready! Exiting...\n\r");
		exit(-1);
	}

	int g;
	int pruebas = 1000;
	double acu_aux;
	for (g = 0; g < pruebas; g++) {
		startTest();
		XFft_Start(&HlsFFT);
		while (!XFft_IsReady(&HlsFFT))
			;
		acu_aux = endTest();
	}
	printf("Run-time = %.2f usec...\n", acu_aux / pruebas);
	print("Detected HLS peripheral complete. Result received.\n\r");

//	for (i = 0; i < 1024; i++) {
//		XFft_Read_real_r_Words(&HlsFFT, i * 2, &unTemp.f64, length);
//		printf("real_f: %.2f\n", unTemp.f64);
//		XFft_Read_img_Words(&HlsFFT, i * 2, &unTemp.f64, length);
//		printf("img_f: %.2f\n", unTemp.f64);
//	}

	cleanup_platform();
	return status;
}

int hls_FFT_init(XFft *hls_FFTPtr) {
	XFft_Config *cfgPtr;
	int status;

	cfgPtr = XFft_LookupConfig(XPAR_XFFT_0_DEVICE_ID);
	if (!cfgPtr) {
		print("ERROR: Lookup of accelerator configuration failed.\n\r");
		return XST_FAILURE;
	}
	status = XFft_CfgInitialize(hls_FFTPtr, cfgPtr);
	if (status != XST_SUCCESS) {
		print("ERROR: Could not initialize accelerator.\n\r");
		return XST_FAILURE;
	}
	return status;
}

void hls_FFT_start(void *InstancePtr) {
	XFft *pAccelerator = (XFft *) InstancePtr;
	XFft_InterruptEnable(pAccelerator, 1);
	XFft_InterruptGlobalEnable(pAccelerator);
	XFft_Start(pAccelerator);
}

void hls_FFT_isr(void *InstancePtr) {
	XFft *pAccelerator = (XFft *) InstancePtr;

	//Disable the global interrupt
	XFft_InterruptGlobalDisable(pAccelerator);
	//Disable the local interrupt
	XFft_InterruptDisable(pAccelerator, 0xffffffff);

	// clear the local interrupt
	XFft_InterruptClear(pAccelerator, 1);

	ResultAvailHlsFFT = 1;
	// restart the core if it should run again
	if (RunHlsFFT) {
		hls_FFT_start(pAccelerator);
	}
}

int setup_interrupt() {
	//This functions sets up the interrupt on the ARM
	int result;
	XScuGic_Config *pCfg = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	if (pCfg == NULL) {
		print("Interrupt Configuration Lookup Failed\n\r");
		return XST_FAILURE;
	}
	result = XScuGic_CfgInitialize(&ScuGic, pCfg, pCfg->CpuBaseAddress);
	if (result != XST_SUCCESS) {
		return result;
	}
	// self test
	result = XScuGic_SelfTest(&ScuGic);
	if (result != XST_SUCCESS) {
		return result;
	}
	// Initialize the exception handler
	Xil_ExceptionInit();
	// Register the exception handler
	//print("Register the exception handler\n\r");
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler, &ScuGic);
	//Enable the exception handler
	Xil_ExceptionEnable();
	// Connect the Adder ISR to the exception table
	//print("Connect the Adder ISR to the Exception handler table\n\r");
	result = XScuGic_Connect(&ScuGic, XPAR_FABRIC_FFT_0_INTERRUPT_INTR,
			(Xil_InterruptHandler) hls_FFT_isr, &HlsFFT);
	if (result != XST_SUCCESS) {
		return result;
	}
	//print("Enable the Adder ISR\n\r");
	XScuGic_Enable(&ScuGic, XPAR_FABRIC_FFT_0_INTERRUPT_INTR);
	return XST_SUCCESS;
}
