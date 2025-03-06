/*******************************************************************************
 Source: MachSuite (please look the LICENSE below for details)
 Modified by: David Berrazueta & Byron Navas
 Associated Filenames: BPNN_BSP.c
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

#include <stdio.h>
#include "platform.h"
// Add BSP header files
#include <stdlib.h>  // Standard C functions, e.g. exit()
#include <stdbool.h> // Provides a Boolean data type for ANSI/ISO-C
#include "xparameters.h" // Parameter definitions for processor periperals
#include "xscugic.h"     // Processor interrupt controller device driver
#include "xbackprop.h"   // Device driver for HLS HW block
#include "xbasic_types.h"

// HLS macc HW instance
XBackprop HlsBackprop;
//Interrupt Controller Instance
XScuGic ScuGic;
// Global variable definitions - used by ISR
volatile static int RunHlsBackprop = 0;
volatile static int ResultAvailHlsBackprop = 0;

// Setup and helper functions
int setup_interrupt();
int hls_backprop_init(XBackprop *hls_BackpropPtr);
void hls_backprop_start(void *InstancePtr);
// The ISR prototype
void hls_bakcprop_isr(void *InstancePtr);
// Software model of HLS hardware


#include "xtime_l.h"
#define TIMEDIFF(t1,t2)  (t2 - t1)
#define MICROSECONDS(t) (1000000.0 * t / COUNTS_PER_SECOND)
XTime start, end;

void startTest() {
	XTime_GetTime(&start);
}

double acumulador=0;

double endTest() {
	XTime_GetTime(&end);
	double time_curr = TIMEDIFF(start, end);
	double usec = MICROSECONDS(time_curr);
	acumulador=acumulador+usec;
	return acumulador;
}



int main() {
	print("Program to test communication with HLS Backprop block in PL\n\r");

	int status;
	//
	int j;
	int length = 2;
	int input_dimension = 13;
	int possible_outputs = 3;
	int training_sets = 163;
	int nodes_per_layer = 64;

	union {
		Xuint32 u32;
		Xfloat32 f32;
		Xuint8 u8[4];
		Xfloat64 f64;
	} unTemp;

	Xfloat64 A = (double) (0.3);
	unTemp.f64 = A;

	//Setup the backprop ip
	status = hls_backprop_init(&HlsBackprop);
	if (status != XST_SUCCESS) {
		print("HLS peripheral setup failed\n\r");
		exit(-1);
	}
	//Setup the interrupt
	status = setup_interrupt();
	if (status != XST_SUCCESS) {
		print("Interrupt setup failed\n\r");
		exit(-1);
	}

	//set the input parameters of the HLS block
	for (j = 0; j < input_dimension * nodes_per_layer; j++) {
		XBackprop_Write_weights1_Words(&HlsBackprop, j*2, &unTemp.f64, length);
	}
	for (j = 0; j < nodes_per_layer * nodes_per_layer; j++) {
		XBackprop_Write_weights2_Words(&HlsBackprop, j*2, &unTemp.f64, length);
	}
	for (j = 0; j < nodes_per_layer * possible_outputs; j++) {
		XBackprop_Write_weights3_Words(&HlsBackprop, j*2, &unTemp.f64, length);
	}
	for (j = 0; j < nodes_per_layer; j++) {
		XBackprop_Write_biases1_Words(&HlsBackprop, j*2, &unTemp.f64, length);
	}
	for (j = 0; j < nodes_per_layer; j++) {
		XBackprop_Write_biases2_Words(&HlsBackprop, j*2, &unTemp.f64, length);
	}
	for (j = 0; j < possible_outputs; j++) {
		XBackprop_Write_biases3_Words(&HlsBackprop, j*2, &unTemp.f64, length);
	}
	for (j = 0; j < training_sets * input_dimension; j++) {
		Xfloat64 A = (double) (j);
		unTemp.f64 = A;
		XBackprop_Write_training_data_Words(&HlsBackprop, j*2, &unTemp.f64,
				length);
	}
	for (j = 0; j < training_sets * possible_outputs; j++) {
		Xfloat64 A = (double) (j);
		unTemp.f64 = A;
		XBackprop_Write_training_targets_Words(&HlsBackprop, j*2, &unTemp.f64,
				length);
	}

	if (XBackprop_IsReady(&HlsBackprop))
		print("HLS peripheral is ready.  Starting... ");
	else {
		print("!!! HLS peripheral is not ready! Exiting...\n\r");
		exit(-1);
	}

	if (0) { // use interrupt
		hls_backprop_start(&HlsBackprop);
		while (!ResultAvailHlsBackprop)
			; // spin
		print("Interrupt received from HLS HW.\n\r");
	} else { // Simple non-interrupt driven test
		int g;
		int pruebas=100;
		double acu_aux;
		for (g = 0; g < pruebas; g++) {
			startTest();
			XBackprop_Start(&HlsBackprop);
			while (!XBackprop_IsReady(&HlsBackprop));
			acu_aux=endTest();
		}
		printf("Run-time = %.2f usec...\n", acu_aux/pruebas);

		print("Detected HLS peripheral complete. Result received.\n\r");
	}

//	for (j = 0; j < input_dimension * nodes_per_layer; j++) {
//		XBackprop_Read_weights1_Words(&HlsBackprop, j*2, &unTemp.f64, length);
//		printf("%.3f\n", unTemp.f64);
//	}
//	for (j = 0; j < nodes_per_layer * nodes_per_layer; j++) {
//		XBackprop_Read_weights2_Words(&HlsBackprop, j*2, &unTemp.f64, length);
//		printf("%.3f\n", unTemp.f64);
//	}
//	for (j = 0; j < nodes_per_layer * possible_outputs; j++) {
//		XBackprop_Read_weights3_Words(&HlsBackprop, j*2, &unTemp.f64, length);
//		printf("%.3f\n", unTemp.f64);
//	}
//	for (j = 0; j < nodes_per_layer; j++) {
//		XBackprop_Read_biases1_Words(&HlsBackprop, j*2, &unTemp.f64, length);
//		printf("%.3f\n", unTemp.f64);
//	}
//	for (j = 0; j < nodes_per_layer; j++) {
//		XBackprop_Read_biases2_Words(&HlsBackprop, j*2, &unTemp.f64, length);
//		printf("%.3f\n", unTemp.f64);
//	}
//	for (j = 0; j < possible_outputs; j++) {
//		XBackprop_Read_biases3_Words(&HlsBackprop, j*2, &unTemp.f64, length);
//		printf("%.3f\n", unTemp.f64);
//	}
//	for (j = 0; j < training_sets * input_dimension; j++) {
//		XBackprop_Read_training_data_Words(&HlsBackprop, j*2, &unTemp.f64,
//				length);
//		printf("%.3f\n", unTemp.f64);
//	}
//	for (j = 0; j < training_sets * possible_outputs; j++) {
//		XBackprop_Read_training_targets_Words(&HlsBackprop, j*2, &unTemp.f64,
//				length);
//		printf("%.3f\n", unTemp.f64);
//	}

	status = 0;

	cleanup_platform();
	return status;
}

int hls_backprop_init(XBackprop *hls_BackpropPtr) {
	XBackprop_Config *cfgPtr;
	int status;

	cfgPtr = XBackprop_LookupConfig(XPAR_XBACKPROP_0_DEVICE_ID);
	if (!cfgPtr) {
		print("ERROR: Lookup of acclerator configuration failed.\n\r");
		return XST_FAILURE;
	}
	status = XBackprop_CfgInitialize(hls_BackpropPtr, cfgPtr);
	if (status != XST_SUCCESS) {
		print("ERROR: Could not initialize accelerator.\n\r");
		return XST_FAILURE;
	}
	return status;
}

void hls_backprop_start(void *InstancePtr) {
	XBackprop *pAccelerator = (XBackprop *) InstancePtr;
	XBackprop_InterruptEnable(pAccelerator, 1);
	XBackprop_InterruptGlobalEnable(pAccelerator);
	XBackprop_Start(pAccelerator);
}

void hls_bakcprop_isr(void *InstancePtr) {
	XBackprop *pAccelerator = (XBackprop *) InstancePtr;

//Disable the global interrupt
	XBackprop_InterruptGlobalDisable(pAccelerator);
//Disable the local interrupt
	XBackprop_InterruptDisable(pAccelerator, 0xffffffff);

// clear the local interrupt
	XBackprop_InterruptClear(pAccelerator, 1);

	ResultAvailHlsBackprop = 1;
// restart the core if it should run again
	if (RunHlsBackprop) {
		hls_backprop_start(pAccelerator);
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
	result = XScuGic_Connect(&ScuGic, XPAR_FABRIC_BACKPROP_0_INTERRUPT_INTR,
			(Xil_InterruptHandler) hls_bakcprop_isr, &HlsBackprop);
	if (result != XST_SUCCESS) {
		return result;
	}
//print("Enable the Adder ISR\n\r");
	XScuGic_Enable(&ScuGic, XPAR_FABRIC_BACKPROP_0_INTERRUPT_INTR);
	return XST_SUCCESS;
}
