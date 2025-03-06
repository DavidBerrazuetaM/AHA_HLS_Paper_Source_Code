/*******************************************************************************
 Source: Custom coded
 Design by: David Berrazueta & Byron Navas
 Associated Filenames: matrixmul_test.cpp, matrixmul.cpp, matrixmul.h
 Purpose: Matrix Multiplier software version for BSP Standalone (ARM Cortex A9)
 Revision History: November 11, 2019
 *******************************************************************************
 Matrix Multiplier v0.1
 *******************************************************************************/
#ifndef __MATRIXMUL_H__
#define __MATRIXMUL_H__

#include <cmath>
using namespace std;

#define MAT_A_ROWS 2
#define MAT_A_COLS 2
#define MAT_B_ROWS 2
#define MAT_B_COLS 2

typedef int mat_a_t;
typedef int mat_b_t;
typedef int result_t;

void matrixmul(mat_a_t a[MAT_A_ROWS][MAT_A_COLS],
		mat_b_t b[MAT_B_ROWS][MAT_B_COLS],
		result_t res[MAT_A_ROWS][MAT_B_COLS]);

#endif
