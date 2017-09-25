#ifndef __KARATSUBAMUL_H_
#define __KARATSUBAMUL_H_
#include "sum.h"
#include "sub.h"

void mult10(char* a, int n);
char* CreateArray(int len);
void make_equal_length(char* a, char* b);
void karatsuba(char* x, char* y, char* res);
void add_zeros(char* a, int n);
void remove_zeros(char* a);

#endif
