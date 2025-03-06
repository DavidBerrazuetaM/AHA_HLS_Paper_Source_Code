/*******************************************************************************
 Source: CHStone (please look the LICENSE below for details)
 Modified by: David Berrazueta & Byron Navas
 Associated Filenames: fcn.cpp, hd.h, tb.cpp, gold.dat
 Purpose: AES software version
 Revision History: November 11, 2019
 *******************************************************************************
 AES v0.1
 *******************************************************************************
 Copyright (C) 2005
 Akira Iwata & Masayuki Sato
 Akira Iwata Laboratory,
 Nagoya Institute of Technology in Japan.

 All rights reserved.

 This software is written by Masayuki Sato.
 And if you want to contact us, send an email to Kimitake Wakayama
 (wakayama@elcom.nitech.ac.jp)

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 3. All advertising materials mentioning features or use of this software must
    display the following acknowledgment:
    "This product includes software developed by Akira Iwata Laboratory,
    Nagoya Institute of Technology in Japan (http://mars.elcom.nitech.ac.jp/)."

 4. Redistributions of any form whatsoever must retain the following
    acknowledgment:
    "This product includes software developed by Akira Iwata Laboratory,
     Nagoya Institute of Technology in Japan (http://mars.elcom.nitech.ac.jp/)."

   THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
   AKIRA IWATA LABORATORY DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
   SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
   IN NO EVENT SHALL AKIRA IWATA LABORATORY BE LIABLE FOR ANY SPECIAL,
   INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING
   FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
   NEGLIGENCE OR OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION
   WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *******************************************************************************/

#include <stdio.h>
#include <cstdlib>
#include "hd.h"


int main() {

	FILE *fp;

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

	// Run the AES main function
	aes_main(statemt, key,enc,dec);

	// Save function results as .dat
	fp = fopen("results.dat", "w");
	for (i = 0; i < 16; ++i) {
		if (enc[i] < 16)
			fprintf(fp,"0");
		fprintf(fp, "%x", enc[i]);
		main_result += (enc[i] != out_enc_statemt[i]);
	}

	fprintf(fp,"\n");
	for (i = 0; i < 16; ++i) {
		if (dec[i] < 16)
			fprintf(fp,"0");
		fprintf(fp, "%x", dec[i]);
		main_result += (dec[i] != out_dec_statemt[i]);
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
