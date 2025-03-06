#include "fft.h"

void fft(double real[FFT_SIZE], double img[FFT_SIZE],
		double real_twid[FFT_SIZE / 2], double img_twid[FFT_SIZE / 2]) {

#pragma HLS INTERFACE s_axilite port=img_twid bundle=FFT_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=real_twid bundle=FFT_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=img bundle=FFT_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=real bundle=FFT_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=return bundle=FFT_PERIPH_BUS

	int even, odd, span, log, rootindex,i;
	double temp;
	log = 0;

	for (span = FFT_SIZE >> 1; span; span >>= 1, log++) {
		odd=span;
		for (i = 0; i < FFT_SIZE/2; i++) {

			odd |= span;
			even = odd ^ span;

			temp = real[even] + real[odd];
			real[odd] = real[even] - real[odd];
			real[even] = temp;

			temp = img[even] + img[odd];
			img[odd] = img[even] - img[odd];
			img[even] = temp;

			rootindex = (even << log) & (FFT_SIZE - 1);
			temp = real_twid[rootindex] * real[odd]
					- img_twid[rootindex] * img[odd];
			img[odd] = real_twid[rootindex] * img[odd]
					+ img_twid[rootindex] * real[odd];
			real[odd] = temp;
			odd++;
		}
	}
}
