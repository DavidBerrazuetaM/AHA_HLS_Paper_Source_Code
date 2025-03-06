/*******************************************************************************
 Source: Custom coded
 Design by: David Berrazueta & Byron Navas
 Associated Filenames: matrixmul_BSP.c
 Purpose: Matrix Multiplier software version for BSP Standalone (ARM Cortex A9)
 Revision History: November 11, 2019
 *******************************************************************************
 Matrix Multiplier v0.1
 *******************************************************************************/
#include <stdio.h>
#include "platform.h"
// Add BSP header files
#include <stdlib.h>  // Standard C functions, e.g. exit()
#include <stdbool.h> // Provides a Boolean data type for ANSI/ISO-C
#include "xparameters.h" // Parameter definitions for processor periperals
#include "xscugic.h"     // Processor interrupt controller device driver
#include "xmatrixmul.h"   // Device driver for HLS HW block
#include "xil_cache.h"
#include "xil_cache_l.h"

// HLS macc HW instance
XMatrixmul Hls_matrixmul;
//Interrupt Controller Instance
XScuGic ScuGic;
// Global variable definitions - used by ISR
volatile static int RunHlsMatrixmul = 0;
volatile static int ResultAvailHlsMatrixmul = 0;

// Setup and helper functions
int setup_interrupt();
int hls_matrixmul_init(XMatrixmul *hls_matrixmulPtr);
void hls_matrixmul_start(void *InstancePtr);
// The ISR prototype
void hls_matrixmul_isr(void *InstancePtr);
// Software model of HLS hardware
int MAT_A_ROWS = 2;
int MAT_A_COLS = 2;
int MAT_B_ROWS = 2;
int MAT_B_COLS = 2;
void sw_matrixmul(int a[MAT_A_ROWS][MAT_A_COLS], int b[MAT_B_ROWS][MAT_B_COLS],
		int res[MAT_A_ROWS][MAT_B_COLS]);

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
	print(
			"Program to test communication with HLS Matrix Multiplier block in PL\n\r");
	int a_0_0 = 0;
	int a_0_1 = 0;
	int a_1_0 = 20000;
	int a_1_1 = 0;

	int a[2][2] = { { a_0_0, a_0_1 }, { a_1_0, a_1_1 } };

	int b_0_0 = 0;
	int b_0_1 = 1;
	int b_1_0 = 0;
	int b_1_1 = 0;

	int b[2][2] = { { b_0_0, b_0_1 }, { b_1_0, b_1_1 } };

	int res_hw_0_0;
	int res_hw_0_1;
	int res_hw_1_0;
	int res_hw_1_1;

	int res_sw[MAT_A_ROWS][MAT_B_COLS];
	int status;

	//Setup the matrix mult
	status = hls_matrixmul_init(&Hls_matrixmul);
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
	XMatrixmul_Set_a_0_0(&Hls_matrixmul, a_0_0);
	XMatrixmul_Set_a_0_1(&Hls_matrixmul, a_0_1);
	XMatrixmul_Set_a_1_0(&Hls_matrixmul, a_1_0);
	XMatrixmul_Set_a_1_1(&Hls_matrixmul, a_1_1);
	XMatrixmul_Set_b_0_0(&Hls_matrixmul, b_0_0);
	XMatrixmul_Set_b_0_1(&Hls_matrixmul, b_0_1);
	XMatrixmul_Set_b_1_0(&Hls_matrixmul, b_1_0);
	XMatrixmul_Set_b_1_1(&Hls_matrixmul, b_1_1);

	if (XMatrixmul_IsReady(&Hls_matrixmul))
		print("HLS peripheral is ready.  Starting... ");
	else {
		print("!!! HLS peripheral is not ready! Exiting...\n\r");
		exit(-1);
	}

	if (0) { // use interrupt
		hls_matrixmul_start(&Hls_matrixmul);
		while (!ResultAvailHlsMatrixmul)
			; // spin
		res_hw_0_0 = XMatrixmul_Get_res_0_0(&Hls_matrixmul);
		res_hw_0_1 = XMatrixmul_Get_res_0_1(&Hls_matrixmul);
		res_hw_1_0 = XMatrixmul_Get_res_1_0(&Hls_matrixmul);
		res_hw_1_1 = XMatrixmul_Get_res_1_1(&Hls_matrixmul);

		print("Interrupt received from HLS HW.\n\r");
	} else { // Simple non-interrupt driven test
		int g;
		int pruebas = 1000;
		double acu_aux;
		for (g = 0; g < pruebas; g++) {
			startTest();
			XMatrixmul_Start(&Hls_matrixmul);
			while (!XMatrixmul_IsReady(&Hls_matrixmul))
				;
			acu_aux = endTest();
		}
		printf("Run-time = %.2f usec...\n", acu_aux / pruebas);
		print("Detected HLS peripheral complete. Result received.\n\r");
	}

	res_hw_0_0 = XMatrixmul_Get_res_0_0(&Hls_matrixmul);
	res_hw_0_1 = XMatrixmul_Get_res_0_1(&Hls_matrixmul);
	res_hw_1_0 = XMatrixmul_Get_res_1_0(&Hls_matrixmul);
	res_hw_1_1 = XMatrixmul_Get_res_1_1(&Hls_matrixmul);

	//call the software version of the function
	sw_matrixmul(a, b, res_sw);

	printf("Result from HW: %d; Result from SW: %d\n\r", res_hw_0_0,
			res_sw[0][0]);
	printf("Result from HW: %d; Result from SW: %d\n\r", res_hw_0_1,
			res_sw[0][1]);
	printf("Result from HW: %d; Result from SW: %d\n\r", res_hw_1_0,
			res_sw[1][0]);
	printf("Result from HW: %d; Result from SW: %d\n\r", res_hw_1_1,
			res_sw[1][1]);
	if ((res_hw_0_0 == res_sw[0][0]) & (res_hw_0_1 == res_sw[0][1])
			& (res_hw_1_0 == res_sw[1][0]) & (res_hw_1_1 == res_sw[1][1])) {
		print("*** Results match ***\n\r");
		status = 0;
	} else {
		print("!!! MISMATCH !!!\n\r");
		status = -1;
	}

	cleanup_platform();
	return status;
}

void sw_matrixmul(int a[MAT_A_ROWS][MAT_A_COLS], int b[MAT_B_ROWS][MAT_B_COLS],
		int sw_result[MAT_A_ROWS][MAT_B_COLS]) {

	int i, j, k;

	for (i = 0; i < MAT_A_ROWS; i++) {
		for (j = 0; j < MAT_B_COLS; j++) {
			sw_result[i][j] = 0;
			for (k = 0; k < MAT_B_ROWS; k++) {
				sw_result[i][j] += a[i][k] * b[k][j];
			}
		}
	}

}

int hls_matrixmul_init(XMatrixmul *hls_matrixmulPtr) {
	XMatrixmul_Config *cfgPtr;
	int status;

	cfgPtr = XMatrixmul_LookupConfig(XPAR_MATRIXMUL_0_DEVICE_ID);
	if (!cfgPtr) {
		print("ERROR: Lookup of acclerator configuration failed.\n\r");
		return XST_FAILURE;
	}
	status = XMatrixmul_CfgInitialize(hls_matrixmulPtr, cfgPtr);
	if (status != XST_SUCCESS) {
		print("ERROR: Could not initialize accelerator.\n\r");
		return XST_FAILURE;
	}
	return status;
}

void hls_matrixmul_start(void *InstancePtr) {
	XMatrixmul *pAccelerator = (XMatrixmul *) InstancePtr;
	XMatrixmul_InterruptEnable(pAccelerator, 1);
	XMatrixmul_InterruptGlobalEnable(pAccelerator);
	XMatrixmul_Start(pAccelerator);
}

void hls_matrixmul_isr(void *InstancePtr) {
	XMatrixmul *pAccelerator = (XMatrixmul *) InstancePtr;

	//Disable the global interrupt
	XMatrixmul_InterruptGlobalDisable(pAccelerator);
	//Disable the local interrupt
	XMatrixmul_InterruptDisable(pAccelerator, 0xffffffff);

	// clear the local interrupt
	XMatrixmul_InterruptClear(pAccelerator, 1);

	ResultAvailHlsMatrixmul = 1;
	// restart the core if it should run again
	if (RunHlsMatrixmul) {
		hls_matrixmul_start(pAccelerator);
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
	result = XScuGic_Connect(&ScuGic, XPAR_FABRIC_MATRIXMUL_0_INTERRUPT_INTR,
			(Xil_InterruptHandler) hls_matrixmul_isr, &Hls_matrixmul);
	if (result != XST_SUCCESS) {
		return result;
	}
	//print("Enable the Adder ISR\n\r");
	XScuGic_Enable(&ScuGic, XPAR_FABRIC_MATRIXMUL_0_INTERRUPT_INTR);
	return XST_SUCCESS;
}
