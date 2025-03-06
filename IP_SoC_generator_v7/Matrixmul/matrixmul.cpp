/*******************************************************************************
 Designer: David Berrazueta M.
 Associated Filenames: matrixmul.cpp, matrixmul.h, matrixmul_test.cpp
 Purpose: Vivado HLS accelerator 1
 Device: xc7z010clg400-1 (Zybo)
 Clock Period: 4ns
 Clock Uncertainty: default (12.5%)
 Revision History: November 6, 2018

 *******************************************************************************
 Matrix Multiplier v0.1

 *******************************************************************************/

#include "matrixmul.h"

void matrixmul(mat_a_t a[MAT_A_ROWS][MAT_A_COLS],
		mat_b_t b[MAT_B_ROWS][MAT_B_COLS],
		result_t res[MAT_A_ROWS][MAT_B_COLS]) {
#pragma HLS INTERFACE s_axilite port=return bundle=MATRIXMUL_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=res bundle=MATRIXMUL_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=b bundle=MATRIXMUL_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=a bundle=MATRIXMUL_PERIPH_BUS
	Row: for (int i = 0; i < MAT_A_ROWS; i++) {
		Col: for (int j = 0; j < MAT_B_COLS; j++) {
			res[i][j] = 0;
			Inner: for (int k = 0; k < MAT_B_ROWS; k++) {
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}

}
