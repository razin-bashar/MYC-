#include "slowdiv.h"
#include "constant.h"
#include "mul.h"

void slowdiv(BigNumber *A, BigNumber *B, BigNumber *C) {
	int i, j, n;
	int na = 0, nb = 0, nab;
    bcd_t* AA = (bcd_t*)malloc(sizeof(bcd_t)*(N_DIGIT + F_DIGIT));

	// AA is a shifted copy of A.
	memcpy(&AA[F_DIGIT], A->digits,
			N_DIGIT * sizeof(bcd_t));
	memset(AA, 0, F_DIGIT * sizeof(bcd_t));

	for (i = N_DIGIT - 1; i >= 0; i--) {
		if ((!na) && (A->digits[i]))
			na = i;
		if ((!nb) && (B->digits[i]))
			nb = i;
	}

	// AA is shifted BigNumber::N_FRAC_DIGITS digits.
	na += F_DIGIT;

	if (!nb && !B->digits[0]) {
		printf("ERROR: division by 0\n");
		exit(255);
	}

	memset(C->digits, 0, N_DIGIT * sizeof(bcd_t));
	C->isPositive = !(A->isPositive ^ B->isPositive);

	if (nb > na)
		return;

	nab = na - nb;

	int menor;
	char r, c;

	i = 1;
	for (i = 0; i <= nab; i++) {

		// Tests if AA is lower than a shifted version of B.
		for (n = 0;; n++) {

			menor = 0;
			for (j = nb + 1; j >= 0; j--) {
				if (AA[j + nab - i] == B->digits[j])
					continue;
				if (AA[j + nab - i] < B->digits[j])
					menor = 1;
				break;
			}
			if (menor)
				break;

			// Substracts a shifted version of B from AA.
			for (c = 0, j = 0; j <= nb + 1; j++) {
				r = AA[j + nab - i] - (B->digits[j] + c);
				c = (r < 0) ? 1 : 0;
				AA[j + nab - i] = (r + 10 * c); // r % 10
			}
		}

		C->digits[nab - i] = n;
	}
}
void slowmod(BigNumber *A, BigNumber *B, BigNumber *C){
    fastMul(B,C,C);//R=A-B*Q
    subtract(A,C,C,1);
}


