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
	Xil_DCacheDisable();//Disable the data cache.
	Xil_ICacheDisable();//Disable the instruction caches.
	Xil_L1ICacheDisable();//Disable the L1 cache
	Xil_L1DCacheDisable();//Disable the level 1 data cache.
	Xil_L2CacheDisable();//Disable the L2 cache.
	print("Program to test communication with HLS MACC block in PL\n\r");
	int status;
	int length = 1, enc[16], dec[16];
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
	XAes_main_Set_statemt_0_i(&HlsAES, statemt[0]);
	XAes_main_Set_statemt_1_i(&HlsAES, statemt[1]);
	XAes_main_Set_statemt_2_i(&HlsAES, statemt[2]);
	XAes_main_Set_statemt_3_i(&HlsAES, statemt[3]);
	XAes_main_Set_statemt_4_i(&HlsAES, statemt[4]);
	XAes_main_Set_statemt_5_i(&HlsAES, statemt[5]);
	XAes_main_Set_statemt_6_i(&HlsAES, statemt[6]);
	XAes_main_Set_statemt_7_i(&HlsAES, statemt[7]);
	XAes_main_Set_statemt_8_i(&HlsAES, statemt[8]);
	XAes_main_Set_statemt_9_i(&HlsAES, statemt[9]);
	XAes_main_Set_statemt_10_i(&HlsAES, statemt[10]);
	XAes_main_Set_statemt_11_i(&HlsAES, statemt[11]);
	XAes_main_Set_statemt_12_i(&HlsAES, statemt[12]);
	XAes_main_Set_statemt_13_i(&HlsAES, statemt[13]);
	XAes_main_Set_statemt_14_i(&HlsAES, statemt[14]);
	XAes_main_Set_statemt_15_i(&HlsAES, statemt[15]);
	XAes_main_Set_statemt_16_i(&HlsAES, statemt[16]);
	XAes_main_Set_statemt_17_i(&HlsAES, statemt[17]);
	XAes_main_Set_statemt_18_i(&HlsAES, statemt[18]);
	XAes_main_Set_statemt_19_i(&HlsAES, statemt[19]);
	XAes_main_Set_statemt_20_i(&HlsAES, statemt[20]);
	XAes_main_Set_statemt_21_i(&HlsAES, statemt[21]);
	XAes_main_Set_statemt_22_i(&HlsAES, statemt[22]);
	XAes_main_Set_statemt_23_i(&HlsAES, statemt[23]);
	XAes_main_Set_statemt_24_i(&HlsAES, statemt[24]);
	XAes_main_Set_statemt_25_i(&HlsAES, statemt[25]);
	XAes_main_Set_statemt_26_i(&HlsAES, statemt[26]);
	XAes_main_Set_statemt_27_i(&HlsAES, statemt[27]);
	XAes_main_Set_statemt_28_i(&HlsAES, statemt[28]);
	XAes_main_Set_statemt_29_i(&HlsAES, statemt[29]);
	XAes_main_Set_statemt_30_i(&HlsAES, statemt[30]);
	XAes_main_Set_statemt_31_i(&HlsAES, statemt[31]);


	XAes_main_Set_key_0(&HlsAES, key[0]);
	XAes_main_Set_key_1(&HlsAES, key[0]);
	XAes_main_Set_key_2(&HlsAES, key[0]);
	XAes_main_Set_key_3(&HlsAES, key[0]);
	XAes_main_Set_key_4(&HlsAES, key[0]);
	XAes_main_Set_key_5(&HlsAES, key[0]);
	XAes_main_Set_key_6(&HlsAES, key[0]);
	XAes_main_Set_key_7(&HlsAES, key[0]);
	XAes_main_Set_key_8(&HlsAES, key[0]);
	XAes_main_Set_key_9(&HlsAES, key[0]);
	XAes_main_Set_key_10(&HlsAES, key[0]);
	XAes_main_Set_key_11(&HlsAES, key[0]);
	XAes_main_Set_key_12(&HlsAES, key[0]);
	XAes_main_Set_key_13(&HlsAES, key[0]);
	XAes_main_Set_key_14(&HlsAES, key[0]);
	XAes_main_Set_key_15(&HlsAES, key[0]);
	XAes_main_Set_key_16(&HlsAES, key[0]);
	XAes_main_Set_key_17(&HlsAES, key[0]);
	XAes_main_Set_key_18(&HlsAES, key[0]);
	XAes_main_Set_key_19(&HlsAES, key[0]);
	XAes_main_Set_key_20(&HlsAES, key[0]);
	XAes_main_Set_key_21(&HlsAES, key[0]);
	XAes_main_Set_key_22(&HlsAES, key[0]);
	XAes_main_Set_key_23(&HlsAES, key[0]);
	XAes_main_Set_key_24(&HlsAES, key[0]);
	XAes_main_Set_key_25(&HlsAES, key[0]);
	XAes_main_Set_key_26(&HlsAES, key[0]);
	XAes_main_Set_key_27(&HlsAES, key[0]);
	XAes_main_Set_key_28(&HlsAES, key[0]);
	XAes_main_Set_key_29(&HlsAES, key[0]);
	XAes_main_Set_key_30(&HlsAES, key[0]);
	XAes_main_Set_key_31(&HlsAES, key[0]);


	if (XAes_main_IsReady(&HlsAES))
		print("HLS peripheral is ready.  Starting... ");
	else {
		print("!!! HLS peripheral is not ready! Exiting...\n\r");
		exit(-1);
	}

	if (0) { // use interrupt
		hls_AES_start(&HlsAES);
		while (!ResultAvailHlsAES)
			; // spin
		print("Interrupt received from HLS HW.\n\r");
	} else { // Simple non-interrupt driven test

		int g;
		double acu_aux;
		for (g = 0; g < 1000; g++) {
		startTest();
		XAes_main_Start(&HlsAES);
		while (!XAes_main_IsReady(&HlsAES));
		acu_aux=endTest();
		}
		printf("Run-time = %.2f usec...\n", acu_aux/1000);

		print("Detected HLS peripheral complete. Result received.\n\r");
	}


	enc[0]=XAes_main_Get_enc_0(&HlsAES);
	enc[1]=XAes_main_Get_enc_1(&HlsAES);
	enc[2]=XAes_main_Get_enc_2(&HlsAES);
	enc[3]=XAes_main_Get_enc_3(&HlsAES);
	enc[4]=XAes_main_Get_enc_4(&HlsAES);
	enc[5]=XAes_main_Get_enc_5(&HlsAES);
	enc[6]=XAes_main_Get_enc_6(&HlsAES);
	enc[7]=XAes_main_Get_enc_7(&HlsAES);
	enc[8]=XAes_main_Get_enc_8(&HlsAES);
	enc[9]=XAes_main_Get_enc_9(&HlsAES);
	enc[10]=XAes_main_Get_enc_10(&HlsAES);
	enc[11]=XAes_main_Get_enc_11(&HlsAES);
	enc[12]=XAes_main_Get_enc_12(&HlsAES);
	enc[13]=XAes_main_Get_enc_13(&HlsAES);
	enc[14]=XAes_main_Get_enc_14(&HlsAES);
	enc[15]=XAes_main_Get_enc_15(&HlsAES);

	int i;
	for (i = 0; i < 16; i++) {
		printf("%x",enc[i]);
	}
	printf("\n");

	dec[0]=XAes_main_Get_dec_0(&HlsAES);
	dec[1]=XAes_main_Get_dec_1(&HlsAES);
	dec[2]=XAes_main_Get_dec_2(&HlsAES);
	dec[3]=XAes_main_Get_dec_3(&HlsAES);
	dec[4]=XAes_main_Get_dec_4(&HlsAES);
	dec[5]=XAes_main_Get_dec_5(&HlsAES);
	dec[6]=XAes_main_Get_dec_6(&HlsAES);
	dec[7]=XAes_main_Get_dec_7(&HlsAES);
	dec[8]=XAes_main_Get_dec_8(&HlsAES);
	dec[9]=XAes_main_Get_dec_9(&HlsAES);
	dec[10]=XAes_main_Get_dec_10(&HlsAES);
	dec[11]=XAes_main_Get_dec_11(&HlsAES);
	dec[12]=XAes_main_Get_dec_12(&HlsAES);
	dec[13]=XAes_main_Get_dec_13(&HlsAES);
	dec[14]=XAes_main_Get_dec_14(&HlsAES);
	dec[15]=XAes_main_Get_dec_15(&HlsAES);


	for (i = 0; i < 16; i++) {
		printf("%x",dec[i]);
	}
	printf("\n");

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
