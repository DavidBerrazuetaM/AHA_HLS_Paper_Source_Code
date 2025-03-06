#include "aes.h"

int decrypt(int statemt[32], int key[32], int type,int dec[32]) {
	int i;

	KeySchedule(type, key);

	switch (type) {
	case 128128:
		round_val = 10;
		nb = 4;
		break;
	case 128192:
	case 192192:
		round_val = 12;
		nb = 6;
		break;
	case 192128:
		round_val = 12;
		nb = 4;
		break;
	case 128256:
	case 192256:
		round_val = 14;
		nb = 8;
		break;
	case 256128:
		round_val = 14;
		nb = 4;
		break;
	case 256192:
		round_val = 14;
		nb = 6;
		break;
	case 256256:
		round_val = 14;
		nb = 8;
		break;
	}

	AddRoundKey(statemt, type, round_val);

	InversShiftRow_ByteSub(statemt, nb);

	for (i = round_val - 1; i >= 1; --i) {
		AddRoundKey_InversMixColumn(statemt, nb, i);
		InversShiftRow_ByteSub(statemt, nb);
	}

	AddRoundKey(statemt, type, 0);


	for (i = 0; i < ((type % 1000) / 8); ++i) {
		dec[i]=statemt[i];
	}

	return 0;
}
