/*******************************************************************************
 Source: Custom coded
 Design by: David Berrazueta & Byron Navas
 Associated Filenames: fcn.cpp, hd.h, tb.cpp, gold.dat
 Purpose: Matrix Multiplier software version
 Revision History: November 11, 2019
 *******************************************************************************
 Matrix Multiplier v0.1
 *******************************************************************************/

#include <stdio.h>
#include <cstdlib>
#include "hd.h"

int main() {

	FILE *fp;

	mat_a_t in_mat_a[2][2] = { {0, 1 }, {1, 2 } };
	mat_b_t in_mat_b[2][2] = { { 3, 5 }, { 8, 13 } };
	result_t hw_result[2][2];

	// Run the matrix multiply main function
	matrixmul(in_mat_a, in_mat_b, hw_result);

	// Save function results as .dat
	fp = fopen("results.dat", "w");
	for (int i = 0; i < MAT_A_ROWS; i++) {
		for (int j = 0; j < MAT_B_COLS; j++) {
			fprintf(fp, "%d\n", (int)hw_result[i][j]);
		}
	}
	fclose(fp);

	// Check function result against golden results
	printf("Comparing against output data ... \n");
	if (system("diff -w results.dat gold.dat")) {
		fprintf(stdout, "**********************************************\n");
		fprintf(stdout, "FAIL: Output DOES NOT match the golden results\n");
		fprintf(stdout, "**********************************************\n");
		return 1;
	} else {
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "PASS: The output matches the golden results!\n");
		fprintf(stdout, "*******************************************\n");
		return 0;
	}
}
