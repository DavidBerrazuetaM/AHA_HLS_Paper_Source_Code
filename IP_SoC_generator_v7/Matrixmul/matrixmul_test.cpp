#include <iostream>
#include <stdio.h>
#include "matrixmul.h"

using namespace std;

int main() {

	FILE *fp;

	mat_a_t in_mat_a[2][2] = { { 0, 0 }, { 0, 1 } };
	mat_b_t in_mat_b[2][2] = { { 1, 1 }, { 0, 1 } };
	result_t hw_result[2][2], sw_result[2][2];
	int err_cnt = 0;

	// Generate the expected result
	for (int i = 0; i < MAT_A_ROWS; i++) {
		for (int j = 0; j < MAT_B_COLS; j++) {
			sw_result[i][j] = (result_t)0;
			for (int k = 0; k < MAT_B_ROWS; k++) {
				sw_result[i][j] += (result_t)in_mat_a[i][k] * (result_t)in_mat_b[k][j];
			}
		}
	}

	// Run the AutoESL matrix multiply block
	matrixmul(in_mat_a, in_mat_b, hw_result);


	fp = fopen("result.dat", "w");
	for (int i = 0; i < MAT_A_ROWS; i++) {
		for (int j = 0; j < MAT_B_COLS; j++) {
			// Check HW result against SW
			if (hw_result[i][j] != sw_result[i][j]) {
				err_cnt++;
			}
				fprintf(fp, "%d %d \n", (int)sw_result[i][j], (int)hw_result[i][j]);
		}
	}
	fclose(fp);

	if (err_cnt)
		cout << "ERROR: " << err_cnt << " mismatches detected!" << endl;
	else
		cout << "Test passes." << endl;
	return err_cnt;
}
