#include "aes.h"



int main() {
	/*
		 +--------------------------------------------------------------------------+
		 | * Test Vectors (added for CHStone)                                       |
		 |     statemt, key : input data                                            |
		 +--------------------------------------------------------------------------+
		 */
		statemt[0] = 50;
		statemt[1] = 67;
		statemt[2] = 246;
		statemt[3] = 168;
		statemt[4] = 136;
		statemt[5] = 90;
		statemt[6] = 48;
		statemt[7] = 141;
		statemt[8] = 49;
		statemt[9] = 49;
		statemt[10] = 152;
		statemt[11] = 162;
		statemt[12] = 224;
		statemt[13] = 55;
		statemt[14] = 7;
		statemt[15] = 52;

		key[0] = 43;
		key[1] = 126;
		key[2] = 21;
		key[3] = 22;
		key[4] = 40;
		key[5] = 174;
		key[6] = 210;
		key[7] = 166;
		key[8] = 171;
		key[9] = 247;
		key[10] = 21;
		key[11] = 136;
		key[12] = 9;
		key[13] = 207;
		key[14] = 79;
		key[15] = 60;

		int j;
		for (j=0;j<32;j++){
			enc[j]=0;
			dec[j]=0;
		}

	const int out_enc_statemt[16] = { 0x39, 0x25, 0x84, 0x1d, 0x2, 0xdc, 0x9,
			0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0xb, 0x32 };
	const int out_dec_statemt[16] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30,
			0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x7, 0x34 };
	int i;

	main_result = 0;

	aes_main(statemt, key,enc,dec);

	printf("encrypted message: \t");
	for (i = 0; i < 16; ++i) {
		if (enc[i] < 16)
			printf("0");
		printf("%x", enc[i]);
		main_result += (enc[i] != out_enc_statemt[i]);
	}

	printf("\ndecrypto message: \t");
	for (i = 0; i < 16; ++i) {
		if (dec[i] < 16)
			printf("0");
		printf("%x", dec[i]);
		main_result += (dec[i] != out_dec_statemt[i]);
	}

	printf("\nmain result: %d\n", main_result);

	return main_result;
}
