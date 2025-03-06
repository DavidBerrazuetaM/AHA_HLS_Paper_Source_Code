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

	encrypt(statemt, key, 128128,enc);
	decrypt(statemt, key, 128128,dec);
	return 0;
}
