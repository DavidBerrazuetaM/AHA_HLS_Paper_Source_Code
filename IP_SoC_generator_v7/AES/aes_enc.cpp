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
