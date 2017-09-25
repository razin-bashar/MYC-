#ifndef __MUL_H_
#define __MUL_H_
#include "bignum.h"

void mulLMA(BigNumber* A, BigNumber* B, BigNumber* C);
void mulFFT(BigNumber* A, BigNumber* B, BigNumber* C);
void fastMul(BigNumber* A, BigNumber* B, BigNumber* C);
void product(char* a, char* b);
#endif
