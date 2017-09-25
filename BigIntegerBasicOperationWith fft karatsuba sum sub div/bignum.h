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

#ifndef __BIGNUMBER_H_
#define __BIGNUMBER_H_

// Arbitrary-precission fixed-point arithmetic.
//
// A BCD system with sign and modulus representation and decimal adjustment
// has been chosen, so each figure represents a decimal digit.

#include "config.h"
#include "constant.h"
struct BigNumbertype {

	bcd_t* digits;
	int isPositive; // positive/!negative flag

};
typedef struct BigNumbertype BigNumber;
void initialize(BigNumber *X,const char* s);
void show(BigNumber* X, int threshold ,int shortNotationDigits);
int compare(BigNumber *A, BigNumber *B);
void copy(BigNumber *A, BigNumber *B);
int equals(BigNumber *A, BigNumber *B);
int length(BigNumber* X);
#endif
