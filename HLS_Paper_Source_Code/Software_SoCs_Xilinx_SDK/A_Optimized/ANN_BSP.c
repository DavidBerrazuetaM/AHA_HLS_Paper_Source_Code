/*******************************************************************************
 Source: Custom coded
 Design by: David Berrazueta & Byron Navas
 Associated Filenames: ANN_BSP.c
 Purpose: ANN software version for BSP Standalone (ARM Cortex A9)
 Revision History: November 11, 2019
 *******************************************************************************
 ANN v0.1
 *******************************************************************************/
#include <stdio.h>
#include "platform.h"
// Add BSP header files
#include <stdlib.h>  // Standard C functions, e.g. exit()
#include <stdbool.h> // Provides a Boolean data type for ANSI/ISO-C
#include "xparameters.h" // Parameter definitions for processor periperals
#include "xscugic.h"     // Processor interrupt controller device driver
#include "xann.h"   // Device driver for HLS HW block
#include "xbasic_types.h"
#include "xil_cache.h"
#include "xil_cache_l.h"

// HLS macc HW instance
XAnn HlsANN;
//Interrupt Controller Instance
XScuGic ScuGic;
// Global variable definitions - used by ISR
volatile static int RunHlsANN = 0;
volatile static int ResultAvailHlsANN = 0;

// Setup and helper functions
int setup_interrupt();
int hls_ANN_init(XAnn *hls_ANNPtr);
void hls_ANN_start(void *InstancePtr);
// The ISR prototype
void hls_ANN_isr(void *InstancePtr);
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
	int length = 1;

	union {
		Xuint32 u32;
		Xfloat32 f32;
		Xuint8 u8[4];
		Xfloat64 f64;
	} unTemp;

	Xfloat32 A = 0;
	unTemp.f32 = A;

	status = hls_ANN_init(&HlsANN);
	if (status != XST_SUCCESS) {
		print("HLS peripheral setup failed\n\r");
		exit(-1);
	}

	double P1[400][1] = { { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 4.881599598488520e-05 }, {
			-2.894297011944075e-04 }, { -3.145000203823761e-03 }, {
			-4.023480494068765e-03 }, { -3.929721576780435e-03 }, {
			-3.957350074507993e-03 }, { -1.777342953813555e-03 }, {
			1.630589865883993e-04 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			000000000000000 }, { 2.425502425502242e-04 }, {
			-2.019880174291773e-03 }, { -6.002365876905656e-03 }, {
			-6.680198120914249e-03 }, { -6.557632080609253e-03 }, {
			-6.365686953921474e-03 }, { -2.072389025054397e-02 }, {
			-3.347341367102331e-02 }, { -3.289981617646993e-02 }, {
			-2.702971813725429e-02 }, { -1.913150707268300e-02 }, {
			-1.243361928104531e-02 }, { -3.352226307189279e-03 }, {
			-3.355290032679511e-03 }, { -4.854473039215357e-03 }, {
			-3.850128408951677e-03 }, { 1.174428104575074e-04 }, {
			000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
			-1.169336757572064e-02 }, { 7.660685593681885e-02 }, {
			6.332082141884533e-01 }, { 7.944101817810459e-01 }, {
			7.748949822984752e-01 }, { 7.683883487559962e-01 }, {
			6.353751531862747e-01 }, { 5.220400837418304e-01 }, {
			5.344494995915037e-01 }, { 4.189294832516343e-01 }, {
			2.750686716127896e-01 }, { 1.547848073257087e-01 }, {
			1.861332380174559e-03 }, { 5.525292755991493e-03 }, {
			2.226276552287614e-02 }, { 2.040836084953756e-02 }, {
			-6.917211328976062e-04 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { -2.717378011495683e-02 }, {
					2.243468137254898e-01 }, { 7.026963167211328e-01 }, {
					8.178328227124181e-01 }, { 8.210775633169932e-01 }, {
					8.904175159322218e-01 }, { 9.294526484204798e-01 }, {
					9.144849196623098e-01 }, { 9.711593477668852e-01 }, {
					1.037693865740742e+00 }, { 1.022847740274212e+00 }, {
					9.605252416939011e-01 }, { 7.374282407407418e-01 }, {
					6.780823461328990e-01 }, { 8.486399441721146e-01 }, {
					6.535811660076377e-01 }, { -1.931559776688439e-02 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 1.522563287269190e-03 }, { -1.480882352941184e-02 }, {
					-5.413960375817019e-03 }, { 4.382465277777767e-02 }, {
					5.143940631808262e-02 }, { 1.524100627777095e-01 }, {
					2.245602362472762e-01 }, { 2.183860464324613e-01 }, {
					2.961358081427009e-01 }, { 4.067629187091493e-01 }, {
					4.619363594363581e-01 }, { 6.416867681100206e-01 }, {
					5.447430044934628e-01 }, { 4.799740604575153e-01 }, {
					5.879819580610008e-01 }, { 3.768139123286174e-01 }, {
					-1.073699618736371e-02 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 2.348049406872993e-04 }, { -3.570991806285954e-03 }, {
					-9.290693408340545e-03 }, { -1.007989890342839e-02 }, {
					-2.077120629156847e-02 }, { -2.830826301414563e-02 }, {
					-2.755574579104016e-02 }, { -3.581223757694380e-02 }, {
					-4.822775263951776e-02 }, { -3.109167059271053e-02 }, {
					2.147955647955600e-02 }, { -9.746545187722104e-03 }, {
					-3.145815498756720e-02 }, { -2.854347560230362e-03 }, {
					-1.650392549255246e-02 }, { 3.750356691533204e-04 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 1.607434833992285e-18 },
			{ 6.467864923747237e-05 }, { -1.794837824249591e-03 }, {
					-7.587826797385635e-03 }, { -4.104370915032686e-03 }, {
					-1.751089324618740e-03 }, { -5.084422657952082e-03 }, {
					-2.103630780101374e-03 }, { 6.808278867102358e-05 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 }, { 000000000000000 }, { 000000000000000 },
			{ 000000000000000 }, { 000000000000000 }, { 000000000000000 }, {
					000000000000000 } };

	int i;
	for (i = 0; i < 400; i++) {
		Xfloat32 A = P1[i][1];
		unTemp.f32 = A;
		XAnn_Write_input_r_Words(&HlsANN, i, &unTemp.f32, length);
	}

	if (XAnn_IsReady(&HlsANN))
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
		XAnn_Start(&HlsANN);
		while (!XAnn_IsReady(&HlsANN))
			;
		acu_aux = endTest();
	}
	printf("Run-time = %.2f usec...\n", acu_aux / pruebas);
	print("Detected HLS peripheral complete. Result received.\n\r");

	int numero = XAnn_Get_return(&HlsANN);
	printf("numero detectado: %i\n", numero);

	cleanup_platform();
	return status;
}

int hls_ANN_init(XAnn *hls_ANNPtr) {
	XAnn_Config *cfgPtr;
	int status;

	cfgPtr = XAnn_LookupConfig(XPAR_XANN_0_DEVICE_ID);
	if (!cfgPtr) {
		print("ERROR: Lookup of accelerator configuration failed.\n\r");
		return XST_FAILURE;
	}
	status = XAnn_CfgInitialize(hls_ANNPtr, cfgPtr);
	if (status != XST_SUCCESS) {
		print("ERROR: Could not initialize accelerator.\n\r");
		return XST_FAILURE;
	}
	return status;
}

void hls_ANN_start(void *InstancePtr) {
	XAnn *pAccelerator = (XAnn *) InstancePtr;
	XAnn_InterruptEnable(pAccelerator, 1);
	XAnn_InterruptGlobalEnable(pAccelerator);
	XAnn_Start(pAccelerator);
}

void hls_ANN_isr(void *InstancePtr) {
	XAnn *pAccelerator = (XAnn *) InstancePtr;

	//Disable the global interrupt
	XAnn_InterruptGlobalDisable(pAccelerator);
	//Disable the local interrupt
	XAnn_InterruptDisable(pAccelerator, 0xffffffff);

	// clear the local interrupt
	XAnn_InterruptClear(pAccelerator, 1);

	ResultAvailHlsANN = 1;
	// restart the core if it should run again
	if (RunHlsANN) {
		hls_ANN_start(pAccelerator);
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
	result = XScuGic_Connect(&ScuGic, XPAR_FABRIC_ANN_0_INTERRUPT_INTR,
			(Xil_InterruptHandler) hls_ANN_isr, &HlsANN);
	if (result != XST_SUCCESS) {
		return result;
	}
	//print("Enable the Adder ISR\n\r");
	XScuGic_Enable(&ScuGic, XPAR_FABRIC_ANN_0_INTERRUPT_INTR);
	return XST_SUCCESS;
}
