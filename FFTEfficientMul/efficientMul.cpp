// efficientMul.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define M_PI 3.14159
#pragma warning(disable:4996)
using namespace std;
struct ComplexNum {

	double r, i;
};

typedef struct ComplexNum Complex;

// Phase factors.
Complex* WN;
long long int N_FRAC_DIGITS = 0;
long long int N_DIGITS = 128;
// Optimization: computes the phase factor table WN[i] = exp(-j*k), with
// k = 0..pi (N samples)
void createPhaseFactors() {

	WN = new Complex[N_DIGITS];
	double alpha;

	for (int i = 0; i < N_DIGITS; i++) {
		alpha = -i * M_PI / N_DIGITS;
		WN[i].r = cos(alpha);
		WN[i].i = sin(alpha);
	}
}

void destroyPhaseFactors() {
	delete[] WN;
}

void fft(Complex *x, Complex *X, unsigned long int N, unsigned long int offset,
	unsigned long int d1, unsigned long int step) {
	Complex X1, X2;
	unsigned long int Np2 = (N >> 1); // N/2
	register unsigned long int a, b, c, q;

	if (N == 2) { // Butterfly for N = 2;

		X1 = x[offset];
		X2 = x[offset + step];
		X[d1].r = X1.r + X2.r;
		X[d1].i = X1.i + X2.i; // X[d1] = X1 + X2
		X[d1 + Np2].r = X1.r - X2.r;
		X[d1 + Np2].i = X1.i - X2.i; // X[d1 + Np2] = X1 - X2
		return;
	}

	fft(x, X, Np2, offset, d1, step << 1);
	fft(x, X, Np2, offset + step, d1 + Np2, step << 1);

	for (q = 0, c = 0; q < (N >> 1); q++, c += step) {

		a = q + d1;
		b = a + Np2;

		X1 = X[a];
		X2.r = X[b].r * WN[c].r - X[b].i * WN[c].i;
		X2.i = X[b].r * WN[c].i + X[b].i * WN[c].r; // X2 = X[b]*WN[c]

		X[a].r = X1.r + X2.r;
		X[a].i = X1.i + X2.i; // X[a] = X1 + X2
		X[b].r = X1.r - X2.r;
		X[b].i = X1.i - X2.i; // X[b] = X1 - X2
	}
}

void ifft(Complex *X, Complex *x, unsigned long int N,
	unsigned long int offset, unsigned long int d1, unsigned long int step) {
	Complex x1, x2;
	unsigned long int Np2 = (N >> 1); // N/2
	double _1pN = 1.0 / N;
	register unsigned long int a, b, c, q;

	if (N == 2) { // Butterfly for N = 2;

		x1 = X[offset];
		x2 = X[offset + step];
		x[d1].r = x1.r + x2.r;
		x[d1].i = x1.i + x2.i; // x[d1] = x1 + x2
		x[d1 + Np2].r = x1.r - x2.r;
		x[d1 + Np2].i = x1.i - x2.i; // x[d1 + Np2] = x1 - x2

		return;
	}

	ifft(X, x, Np2, offset, d1, step << 1);
	ifft(X, x, Np2, offset + step, d1 + Np2, step << 1);

	for (q = 0, c = 0; q < (N >> 1); q++, c += step) {

		a = q + d1;
		b = a + Np2;

		x1 = x[a];
		x2.r = x[b].r * WN[c].r + x[b].i * WN[c].i;
		x2.i = x[b].i * WN[c].r - x[b].r * WN[c].i; // x2 = x[b]*WN*[c]

		x[a].r = x1.r + x2.r;
		x[a].i = x1.i + x2.i; // x[a] = x1 + x2
		x[b].r = x1.r - x2.r;
		x[b].i = x1.i - x2.i; // x[b] = x1 - x2
	}

	if (step != 1)
		return;

	_1pN = 1.0 / N;
	for (q = 0; q < N; q++) {
		x[q].r = x[q].r * _1pN;
		x[q].i = x[q].i * _1pN; // x[q] = x[q]/N
	}
}

void mulFFT(char* A, char* B, char* C) {

	printf("\n%s\n", A);
	printf("\n%s\n", B);

	static Complex* BC1 = new Complex[N_DIGITS << 1];
	static Complex* BC2 = new Complex[N_DIGITS << 1];
	register int i;
	Complex Xi, Xmi, X1, X2, X3;

	// step 1: building a complex signal with the information of both signals.
	for (i = 0; i < N_DIGITS; i++) {
		BC1[i].r = A[i]; // real part
		BC1[i].i = B[i]; // imaginary part
	}

	// cleans the higher section.
	memset(&BC1[N_DIGITS], 0, N_DIGITS * sizeof(Complex));

	// step 2: transform.
	fft(BC1, BC2, (N_DIGITS << 1), 0, 0, 1);

	// step 3: point-wise multiplication in frequency domain.
	for (i = 0; i < (N_DIGITS << 1); i++) {

		// we need to extract the individual transformed signals from the
		// composited one.
		Xi = BC2[i];
		Xmi = BC2[(-i) & ((N_DIGITS << 1) - 1)];
		Xmi.i = -Xmi.i; // conjugate

		X1.r = Xi.r + Xmi.r;
		X1.i = Xi.i + Xmi.i; // X1 = Xi + Xmi
		X2.r = Xi.r - Xmi.r;
		X2.i = Xi.i - Xmi.i; // X2 = Xi - Xmi

		// now let us multiply sample by sample.
		X3.r = X1.r * X2.r - X1.i * X2.i;
		X3.i = X1.r * X2.i + X1.i * X2.r; // X3 = X1*X2;

		BC1[i].r = 0.25 * X3.i;
		BC1[i].i = -0.25 * X3.r;
	}

	// step 4: inverse transform.
	ifft(BC1, BC2, (N_DIGITS << 1), 0, 0, 1);

	unsigned long int c, ci;
	double x;

	// step 5: cleaning and BCD adjust.
	for (i = 0, c = 0; i < N_DIGITS + N_FRAC_DIGITS;
		i++) {

		x = BC2[i].r; // drops imaginary part.

		// rounding
		ci = (unsigned long int) (c + round(x));

		c = (ci / 10); // carry propagation
		if (i >= N_FRAC_DIGITS)
			C[i - N_FRAC_DIGITS] = (ci - 10 * c); // ci % 10
	}

	printf("\n%s\n", C);
}

int main()
{

	char A[4000];
	char B[4000];
	char C[4000];
	strcpy(A, "1111111");
	strcpy(B, "2222222222");
	mulFFT(A, B, C);
	return 0;
}

