/*******************************************************************************
 Source: CHStone (please look the LICENSE below for details)
 Modified by: David Berrazueta & Byron Navas
 Associated Filenames: aes_dec.cpp, aes_enc.cpp, aes_func.cpp, aes_key.cpp,
 											 aes_test.cpp, aes.cpp, aes.h
 Purpose: AES software version for BSP Standalone (ARM Cortex A9)
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
#include "aes.h"

/*  ******* encrypto ************ */
int encrypt(int statemt[32], int key[32], int type,int enc[32]) {

	int i;

	KeySchedule(type, key);
	switch (type) {
	case 128128:
		round_val = 0;
		nb = 4;
		break;
	case 192128:
		round_val = 2;
		nb = 4;
		break;
	case 256128:
		round_val = 4;
		nb = 4;
		break;
	case 128192:
	case 192192:
		round_val = 2;
		nb = 6;
		break;
	case 256192:
		round_val = 4;
		nb = 6;
		break;
	case 128256:
	case 192256:
	case 256256:
		round_val = 4;
		nb = 8;
		break;
	}
	AddRoundKey(statemt, type, 0);
	for (i = 1; i <= round_val + 9; ++i) {
		ByteSub_ShiftRow(statemt, nb);
		MixColumn_AddRoundKey(statemt, nb, i);
	}
	ByteSub_ShiftRow(statemt, nb);
	AddRoundKey(statemt, type, i);

	//for (i = 0; i < nb * 4; ++i) {
	for (i = 0; i < 4 * 4; ++i) {
		enc[i] = statemt[i];
	}

	return 0;
}
