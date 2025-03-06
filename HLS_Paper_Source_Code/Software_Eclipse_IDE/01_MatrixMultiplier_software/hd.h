/*******************************************************************************
 Source: Custom coded
 Design by: David Berrazueta & Byron Navas
 Associated Filenames: fcn.cpp, hd.h, tb.cpp, gold.dat
 Purpose: Matrix Multiplier software version
 Revision History: November 11, 2019
 *******************************************************************************
 Matrix Multiplier v0.1
 *******************************************************************************/

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
