#include "fft.h"
#include <math.h>

int main() {
	double data_x[FFT_SIZE];
	double data_y[FFT_SIZE];
	double img[FFT_SIZE / 2];
	double real[FFT_SIZE / 2];
	int i;
	FILE *fp;

	//set up twiddles...
	double typed;
	int n, N;
	N = FFT_SIZE;

	//Pre-calc twiddles
	for (n = 0; n < (N >> 1); n++) {
		typed = (double) (twoPI * n / N);
		real[n] = cos(typed);
		img[n] = (-1.0) * sin(typed);
	}

	//Init data
	for (i = 0; i < FFT_SIZE; i++) {
		data_x[i] = (double) (i);
		data_y[i] = (double) (i);
	}

	fft(data_x, data_y, real, img);

	fp = fopen("out.dat", "w");
	for (i = 0; i < FFT_SIZE; i++) {
		fprintf(fp, "%.2f : %.2f\n", data_x[i], data_y[i]);
	}
	fclose(fp);

	printf("Comparing against output data \n");
	if (system("diff -w out.dat out.gold.dat")) {

		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
		fprintf(stdout, "*******************************************\n");
		return 1;
	} else {
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "PASS: The output matches the golden output!\n");
		fprintf(stdout, "*******************************************\n");
		return 0;
	}
}
