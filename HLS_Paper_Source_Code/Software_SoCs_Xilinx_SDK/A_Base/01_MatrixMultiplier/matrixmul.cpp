/*******************************************************************************
 Source: Custom coded
 Design by: David Berrazueta & Byron Navas
 Associated Filenames: matrixmul_test.cpp, matrixmul.cpp, matrixmul.h
 Purpose: Matrix Multiplier software version for BSP Standalone (ARM Cortex A9)
 Revision History: November 11, 2019
 *******************************************************************************
 Matrix Multiplier v0.1
 *******************************************************************************/

#include "matrixmul.h"

void matrixmul(mat_a_t a[MAT_A_ROWS][MAT_A_COLS],
		mat_b_t b[MAT_B_ROWS][MAT_B_COLS],
		result_t res[MAT_A_ROWS][MAT_B_COLS]) {

	for (int i = 0; i < MAT_A_ROWS; i++) {
		for (int j = 0; j < MAT_B_COLS; j++) {
			res[i][j] = 0;
			for (int k = 0; k < MAT_B_ROWS; k++) {
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}

}
