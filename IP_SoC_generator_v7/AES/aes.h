/* ************* data type define ************************* */
#include <stdio.h>

extern int type;
extern int nb;
extern int round_val;
extern int key[32];
extern int statemt[32];
extern int word[4][120];
extern int main_result;
extern int enc[32];
extern int dec[32];

int aes_main(int statemt[32],int key[32],int enc[32],int dec[32]);
int decrypt(int statemt[32], int key[32], int type,int dec[32]);
int encrypt(int statemt[32], int key[32], int type,int enc[32]);
int KeySchedule(int type, int key[32]);
void ByteSub_ShiftRow(int statemt[32], int nb);
int SubByte(int in);
void InversShiftRow_ByteSub(int statemt[32], int nb);
int MixColumn_AddRoundKey(int statemt[32], int nb, int n);
int AddRoundKey_InversMixColumn(int statemt[32], int nb, int n);
int AddRoundKey(int statemt[32], int type, int n);
