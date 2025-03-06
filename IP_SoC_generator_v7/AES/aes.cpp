#include "aes.h"

 int nb;
 int round_val;
 int word[4][120];
 int type;
 int key[32];
 int statemt[32];
 int main_result;
 int enc[32];
 int dec[32];

/* ***************** main **************************** */
int aes_main(int statemt[32],int key[32],int enc[32],int dec[32]) {
#pragma HLS INTERFACE s_axilite port=dec bundle=AES_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=enc bundle=AES_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=key bundle=AES_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=statemt bundle=AES_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=return bundle=AES_PERIPH_BUS

	encrypt(statemt, key, 128128,enc);
	decrypt(statemt, key, 128128,dec);
	return 0;
}
