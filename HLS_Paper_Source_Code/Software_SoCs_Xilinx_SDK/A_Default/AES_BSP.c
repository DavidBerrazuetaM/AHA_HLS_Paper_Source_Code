/*******************************************************************************
 Source: CHStone (please look the LICENSE below for details)
 Modified by: David Berrazueta & Byron Navas
 Associated Filenames: AES_BSP.c
 Purpose: AES software version for BSP Standalone (ARM Cortex A9)
 Revision History: November 11, 2019
 *******************************************************************************
 AES v0.1
 *******************************************************************************
 Copyright (C) 2005
 Akira Iwata & Masayuki Sato
 Akira Iwata Laboratory,
 Nagoya Institute of Technology in Japan.

 All rights reserved.

 This software is written by Masayuki Sato.
 And if you want to contact us, send an email to Kimitake Wakayama
 (wakayama@elcom.nitech.ac.jp)

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 3. All advertising materials mentioning features or use of this software must
    display the following acknowledgment:
    "This product includes software developed by Akira Iwata Laboratory,
    Nagoya Institute of Technology in Japan (http://mars.elcom.nitech.ac.jp/)."

 4. Redistributions of any form whatsoever must retain the following
    acknowledgment:
    "This product includes software developed by Akira Iwata Laboratory,
     Nagoya Institute of Technology in Japan (http://mars.elcom.nitech.ac.jp/)."

   THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
   AKIRA IWATA LABORATORY DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
   SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
   IN NO EVENT SHALL AKIRA IWATA LABORATORY BE LIABLE FOR ANY SPECIAL,
   INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING
   FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
   NEGLIGENCE OR OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION
   WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *******************************************************************************/
#include <stdio.h>
#include "platform.h"
// Add BSP header files
#include <stdlib.h>  // Standard C functions, e.g. exit()
#include <stdbool.h> // Provides a Boolean data type for ANSI/ISO-C
#include "xparameters.h" // Parameter definitions for processor periperals
#include "xscugic.h"     // Processor interrupt controller device driver
#include "Xaes_main.h"   // Device driver for HLS HW block
#include "xil_cache.h"
#include "xil_cache_l.h"
// HLS macc HW instance
XAes_main HlsAES;
//Interrupt Controller Instance
XScuGic ScuGic;
// Global variable definitions - used by ISR
volatile static int RunHlsAES = 0;
volatile static int ResultAvailHlsAES = 0;

// Setup and helper functions
int setup_interrupt();
int hls_AES_init(XAes_main *hls_AESPtr);
void hls_AES_start(void *InstancePtr);
// The ISR prototype
void hls_AES_isr(void *InstancePtr);
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
	Xil_DCacheDisable();
	Xil_ICacheDisable();
	Xil_L1ICacheDisable();
	Xil_L2CacheDisable();
	print("Program to test communication with HLS MACC block in PL\n\r");
	int status;
	int length = 1, buffer = 0;
	int statemt[32] = { 50, 67, 246, 168, 136, 90, 48, 141, 49, 49, 152, 162,
			224, 55, 7, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int key[32] = { 43, 126, 21, 22, 40, 174, 210, 166, 171, 247, 21, 136, 9,
			207, 79, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	const int out_enc_statemt[16] = { 0x39, 0x25, 0x84, 0x1d, 0x2, 0xdc, 0x9,
			0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0xb, 0x32 };
	const int out_dec_statemt[16] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30,
			0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x7, 0x34 };

	//Setup the matrix mult
	status = hls_AES_init(&HlsAES);
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
	int i;
	for (i = 0; i < 32; i++) {
		XAes_main_Write_statemt_Words(&HlsAES, i, &statemt[i], length);
	}

	for (i = 0; i < 32; i++) {
		XAes_main_Read_statemt_Words(&HlsAES, i, &buffer, length);
		printf("statemt: %d\n", buffer);
	}

	for (i = 0; i < 32; i++) {
		XAes_main_Write_key_Words(&HlsAES, i, &key[i], length);
	}

	for (i = 0; i < 32; i++) {
		XAes_main_Read_key_Words(&HlsAES, i, &buffer, length);
		printf("key: %d\n", buffer);
	}

/*	if (XAes_main_IsReady(&HlsAES))
		print("HLS peripheral is ready.  Starting... ");
	else {
		print("!!! HLS peripheral is not ready! Exiting...\n\r");
		exit(-1);
	}*/

	if (0) { // use interrupt
		hls_AES_start(&HlsAES);
		while (!ResultAvailHlsAES)
			; // spin
		print("Interrupt received from HLS HW.\n\r");
	} else { // Simple non-interrupt driven test
		int g;
				int pruebas = 1000;
				double acu_aux;
				for (g = 0; g < pruebas; g++) {
		startTest();
		XAes_main_Start(&HlsAES);
		while (!XAes_main_IsReady(&HlsAES));
		acu_aux = endTest();
				}
				printf("Run-time = %.2f usec...\n", acu_aux / pruebas);
		print("Detected HLS peripheral complete. Result received.\n\r");
	}

	for (i = 0; i < 16; i++) {
		XAes_main_Read_enc_Words(&HlsAES, i, &buffer, length);
		printf("enc: %x\n", buffer);
		status += (buffer != out_enc_statemt[i]);
	}

	for (i = 0; i < 16; i++) {
		XAes_main_Read_dec_Words(&HlsAES, i, &buffer, length);
		printf("dec: %x\n", buffer);
		status += (buffer != out_dec_statemt[i]);
	}

	if(status==0){
		printf("*****PASS*****");
	}
	else
		printf("*****FAIL*****");

	cleanup_platform();
	return status;
}

int hls_AES_init(XAes_main *hls_AESPtr) {
	XAes_main_Config *cfgPtr;
	int status;

	cfgPtr = XAes_main_LookupConfig(XPAR_XAES_MAIN_0_DEVICE_ID);
	if (!cfgPtr) {
		print("ERROR: Lookup of accelerator configuration failed.\n\r");
		return XST_FAILURE;
	}
	status = XAes_main_CfgInitialize(hls_AESPtr, cfgPtr);
	if (status != XST_SUCCESS) {
		print("ERROR: Could not initialize accelerator.\n\r");
		return XST_FAILURE;
	}
	return status;
}

void hls_AES_start(void *InstancePtr) {
	XAes_main *pAccelerator = (XAes_main *) InstancePtr;
	XAes_main_InterruptEnable(pAccelerator, 1);
	XAes_main_InterruptGlobalEnable(pAccelerator);
	XAes_main_Start(pAccelerator);
}

void hls_AES_isr(void *InstancePtr) {
	XAes_main *pAccelerator = (XAes_main *) InstancePtr;

	//Disable the global interrupt
	XAes_main_InterruptGlobalDisable(pAccelerator);
	//Disable the local interrupt
	XAes_main_InterruptDisable(pAccelerator, 0xffffffff);

	// clear the local interrupt
	XAes_main_InterruptClear(pAccelerator, 1);

	ResultAvailHlsAES = 1;
	// restart the core if it should run again
	if (RunHlsAES) {
		hls_AES_start(pAccelerator);
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
	result = XScuGic_Connect(&ScuGic, XPAR_FABRIC_AES_MAIN_0_INTERRUPT_INTR,
			(Xil_InterruptHandler) hls_AES_isr, &HlsAES);
	if (result != XST_SUCCESS) {
		return result;
	}
	//print("Enable the Adder ISR\n\r");
	XScuGic_Enable(&ScuGic, XPAR_FABRIC_AES_MAIN_0_INTERRUPT_INTR);
	return XST_SUCCESS;
}
