/*
 BigNumbers - Arbitrary precision arithmetic
 Copyright 2000-2010, Ibán Cereijo Graña <ibancg at gmail dot com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdlib.h>

#include "bignum.h"


// Generic functions implementation

// Constructs an empty BN.
void initialize(BigNumber *X,const char* s){
    X->digits = (bcd_t*)malloc(sizeof(bcd_t)*(N_DIGIT));
	memset(X->digits, 0, N_DIGIT * sizeof(bcd_t));

	// if the first char is the minus sign, the number is negative
	X->isPositive = (*s != '-');

	if (!X->isPositive)
		s++;

	const char *e = strchr(s, 'e');
	unsigned int ls = ((e) ? (e - s) : strlen(s));

	char *bp;
	int exponent = ((e) ? strtol(&e[1], &bp, 0) : 0);

	const char *dot = strchr(s, '.');
	unsigned int dot_index = ((dot) ? (dot - s) : ls); // dot index.
	unsigned int i;

	for (i = 0; i < dot_index; i++)
		X->digits[F_DIGIT + dot_index - i - 1 + exponent] = s[i] - 48;

	int index;
	if (dot_index != ls)
		for (i = dot_index + 1; i < ls; i++) {
			index = F_DIGIT - (i - dot_index) + exponent;
			if (index > 0) {
				X->digits[index] = s[i] - 48;
			}
		}
}
void show(BigNumber* X, int threshold ,
    int shortNotationDigits) {
	int i, j;
	long int ni = 0; // number of integer digits
	long int nf = 0; // number of fractional digits
	int z = 1;

	if (!X->isPositive) {
		printf("-");
	}

	int firstNonZeroIndex = N_DIGIT;

	for (i = N_DIGIT - 1; i >= F_DIGIT; i--) {

		if (z && (X->digits[i])) {
			firstNonZeroIndex = i;
			z = 0;
		}
	}

	if (z) { // special case: zero.
		printf("0");
		ni = 0;
	} else {

		ni = firstNonZeroIndex - F_DIGIT + 1;
		if ((threshold > 0) && (ni > threshold)
				&& (threshold > 2 * shortNotationDigits)) {
			for (i = firstNonZeroIndex;
					i > firstNonZeroIndex - shortNotationDigits; i--) {
				printf("%c",(char) (X->digits[i] + 48));
			}
			printf("...");
			for (i = F_DIGIT + shortNotationDigits - 1;
					i >= F_DIGIT; i--) {
				//ostream << (char) (X.digits[i] + 48);
				printf("%c",(char) (X->digits[i] + 48));
			}

		} else {
			for (i = firstNonZeroIndex; i >= F_DIGIT; i--) {
				//ostream << (char) (X.digits[i] + 48);
				printf("%c",(char) (X->digits[i] + 48));
			}
		}

	}

	for (j = 0; j < F_DIGIT; j++)
		if (X->digits[j])
			break;

	nf = F_DIGIT - j;

	if (nf > 0) {
		// decimal part.
		printf(".");

		nf = F_DIGIT - j;
		if ((nf > threshold) && (threshold > 2 * shortNotationDigits)) {
			for (i = F_DIGIT - 1;
					i >= F_DIGIT - shortNotationDigits; i--) {
				//ostream << (char) (X.digits[i] + 48);
				printf("%c",(char) (X->digits[i] + 48));
			}
			printf( "...");
			for (i = j + shortNotationDigits - 1; i >= j; i--) {
				//ostream << (char) (X.digits[i] + 48);
				printf("%c",(char) (X->digits[i] + 48));
			}
		} else {
			for (i = F_DIGIT - 1; i >= j; i--) {
				//ostream << (char) (X.digits[i] + 48);
				printf("%c",(char) (X->digits[i] + 48));
			}
		}
	}

	//ostream << "::(" << (ni + nf) << " digits, " << ni << " integer and " << nf
	//		<< " fractional)" << std::endl;
    printf("::(%ld digits, %ld integer and %ld fractional)",(ni+nf),ni,nf);
}

int length(BigNumber* X) {
	int i, j;
	long int ni = 0; // number of integer digits
	long int nf = 0; // number of fractional digits
	int z = 1;

	/*if (!X->isPositive) {
		printf("-");
	}*/

	int firstNonZeroIndex = N_DIGIT;

	for (i = N_DIGIT - 1; i >= F_DIGIT; i--) {

		if (z && (X->digits[i])) {
			firstNonZeroIndex = i;
			z = 0;
		}
	}

	if (z) { // special case: zero.
		//printf("0");
		ni = 0;
	} else {

		ni = firstNonZeroIndex - F_DIGIT + 1;
		/*if ((threshold > 0) && (ni > threshold)
				&& (threshold > 2 * shortNotationDigits)) {
			/*for (i = firstNonZeroIndex;
					i > firstNonZeroIndex - shortNotationDigits; i--) {
				printf("%c",(char) (X->digits[i] + 48));
			}
			printf("...");
			for (i = F_DIGIT + shortNotationDigits - 1;
					i >= F_DIGIT; i--) {
				//ostream << (char) (X.digits[i] + 48);
				printf("%c",(char) (X->digits[i] + 48));
			}

		} else {
			for (i = firstNonZeroIndex; i >= F_DIGIT; i--) {
				//ostream << (char) (X.digits[i] + 48);
				printf("%c",(char) (X->digits[i] + 48));
			}
		}*/

	}

	/*for (j = 0; j < F_DIGIT; j++)
		if (X->digits[j])
			break;

	nf = F_DIGIT - j;

	if (nf > 0) {
		// decimal part.
		printf(".");

		nf = F_DIGIT - j;
		if ((nf > threshold) && (threshold > 2 * shortNotationDigits)) {
			for (i = F_DIGIT - 1;
					i >= F_DIGIT - shortNotationDigits; i--) {
				//ostream << (char) (X.digits[i] + 48);
				printf("%c",(char) (X->digits[i] + 48));
			}
			printf( "...");
			for (i = j + shortNotationDigits - 1; i >= j; i--) {
				//ostream << (char) (X.digits[i] + 48);
				printf("%c",(char) (X->digits[i] + 48));
			}
		} else {
			for (i = F_DIGIT - 1; i >= j; i--) {
				//ostream << (char) (X.digits[i] + 48);
				printf("%c",(char) (X->digits[i] + 48));
			}
		}
	}*/

	//ostream << "::(" << (ni + nf) << " digits, " << ni << " integer and " << nf
	//		<< " fractional)" << std::endl;
   // printf("::(%ld digits, %ld integer and %ld fractional)",(ni+nf),ni,nf);
   return ni;
}


int equals(BigNumber *A, BigNumber *B) {
     register int i;
	for ( i = 0; i < N_DIGIT; i++)
		if (A->digits[i] != B->digits[i])
			return 0;
	return 1;
}
void copy(BigNumber *A, BigNumber *B) {
	memcpy(B->digits, A->digits, N_DIGIT * sizeof(bcd_t));
	B->isPositive = A->isPositive;
}


int compare(BigNumber *A, BigNumber *B) {
    register int i;
	for (i = N_DIGIT - 1; i >= 0; i--)
		if (A->digits[i] != B->digits[i])
			return (N_DIGIT - i);

	return N_DIGIT;
}





