#ifndef __DIVIDE_H_
#define __DIVIDE_H_
#include "constant.h"
struct MBigInteger{
 /**
     * Holds the magnitude of this MutableBigInteger in big endian order.
     * The magnitude may start at an offset into the value array, and it may
     * end before the length of the value array.
     */
    int* value;
    int size;
    /**
     * The number of ints of the value array that are currently used
     * to hold the magnitude of this MutableBigInteger. The magnitude starts
     * at an offset and offset + intLen may be less than value.length.
     */
    int intLen;

    /**
     * The offset into the value array where the magnitude of this
     * MutableBigInteger begins.
     */
    int offset;


};
typedef struct MBigInteger MutableBigInteger;

void reset(MutableBigInteger *this);
void copyOfRange(int* des,int* src,int start,int end);
int isZero(MutableBigInteger this) ;
void add(MutableBigInteger *this,MutableBigInteger addend);
void addShifted(MutableBigInteger* this,MutableBigInteger addend, int n);
int compare(MutableBigInteger* this,MutableBigInteger b);
void clear(MutableBigInteger* this);
int numberOfLeadingZeros(int i);
long divWord(long n, int d);
void normalize(MutableBigInteger* this);
int getLowestSetBit(MutableBigInteger* this);
void copyAndShift(int *src, int srcFrom, int srcLen,int* dst, int dstFrom, int shift);
int unsignedLongCompare(long long int one, long long int two);
int mulsubBorrow(int *q, int *a, int x, int len, int offset);
void addDisjoint(MutableBigInteger* this,MutableBigInteger addend, int n);
void primitiveLeftShift(MutableBigInteger* this,int n);
void primitiveRightShift(MutableBigInteger* this,int n);
void setValue(MutableBigInteger* this,int* val,int size, int length);
void safeLeftShift(MutableBigInteger* this,int n);
void leftShift(MutableBigInteger* this,int n);
int bitCount(int i);
int bitLengthForInt(int n);
int bitLength();
int mulsub(int *q, int *a, int x, int len, int offset);
int divadd(int *a,int asize, int* result, int offset);
MutableBigInteger divideMagnitude(MutableBigInteger* this,MutableBigInteger div,MutableBigInteger quotient, int needRemainder);
void rightShift(MutableBigInteger* this,int n);
void safeRightShift(MutableBigInteger* this,int n);
void keepLower(MutableBigInteger *this,int n);
int compareShifted(MutableBigInteger* this,MutableBigInteger b, int ints);
void ones(MutableBigInteger* this,int n);
int subtract(MutableBigInteger* this,MutableBigInteger b);
int divideOneWord(MutableBigInteger* this,int divisor, MutableBigInteger quotient);
MutableBigInteger divide(MutableBigInteger this,MutableBigInteger b,MutableBigInteger quotient, int needRemainder);
MutableBigInteger divideKnuth(MutableBigInteger this,MutableBigInteger b,MutableBigInteger quotient, int needRemainder);
MutableBigInteger divideAndRemainderBurnikelZiegler(MutableBigInteger this,MutableBigInteger b, MutableBigInteger quotient);
MutableBigInteger divide2n1n(MutableBigInteger* this,MutableBigInteger b,MutableBigInteger quotient);
MutableBigInteger divide3n2n(MutableBigInteger* this,MutableBigInteger b,MutableBigInteger quotient);
MutableBigInteger getBlock(int index, int numBlocks,int blockLength);
//long bitLength();
#endif
