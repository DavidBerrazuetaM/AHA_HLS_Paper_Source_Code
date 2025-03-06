/*******************************************************************************
 Source: Custom coded
 Design by: David Berrazueta & Byron Navas
 Associated Filenames: fcn.cpp, hd.h, tb.cpp, gold.dat
 Purpose: Matrix Multiplier software version
 Revision History: November 11, 2019
 *******************************************************************************
 Matrix Multiplier v0.1
 *******************************************************************************/

#include "hd.h"

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
