
#include "divide.h"

int isZero(MutableBigInteger this) {
        return (this.intLen == 0);
    }

void arraycopy(int* src, int startsrc, int* dest, int startdest, int length){
   int i;
   for(i=0;i<length;i++){
    dest[startdest+i] = src[startsrc+i];
   }
}
void add(MutableBigInteger* this,MutableBigInteger addend) {
        int x = this->intLen;
        int y = addend.intLen;
        int resultLen = (this->intLen > addend.intLen) ? this->intLen : addend.intLen;

         int* result = (this->size < resultLen) ? (int*)malloc(sizeof(int)*resultLen): this->value;
         int resultlength = (this->size < resultLen)?resultLen:this->size;
         int rstart = resultlength - 1;

        unsigned long sum;
        long carry = 0;

        // Add common parts of both numbers
        while (x > 0 && y > 0) {
            x--;
            y--;
            sum = (this->value[x + this->offset] & LONG_MASK)
                    + (addend.value[y + addend.offset] & LONG_MASK) + carry;
            result[rstart--] = (int) sum;
            carry = sum >> 32;
        }

        // Add remainder of the longer number
        while (x > 0) {
            x--;
            if (carry == 0 && result == this->value && rstart == (x + this->offset))
                return;
            sum = (this->value[x + this->offset] & LONG_MASK) + carry;
            result[rstart--] = (int) sum;
            carry = sum >> 32;
        }
        while (y > 0) {
            y--;
            sum = (addend.value[y + addend.offset] & LONG_MASK) + carry;
            result[rstart--] = (int) sum;
            carry = sum >> 32;
        }

        if (carry > 0) { // Result must grow in length
            resultLen++;
            if (resultlength < resultLen) {
                int* temp = (int*) malloc(sizeof(int)*resultLen);
                // Result one word longer from carry-out; copy low-order
                // bits into new result.
                arraycopy(result, 0, temp, 1, resultlength);
                temp[0] = 1;
                result = temp;
            } else {
                result[rstart--] = 1;
            }
        }

        this->value = result;
        this->size = resultlength;
        this->intLen = resultLen;
        this->offset = resultlength - resultLen;
    }

void addShifted(MutableBigInteger* this,MutableBigInteger addend, int n) {
        if (isZero(addend)) {
            return;
        }

        int x = this->intLen;
        int y = addend.intLen + n;
        int resultLen = (this->intLen > y ? this->intLen : y);
      //  int[] result = (value.length < resultLen ? new int[resultLen]
        //        : value);

        //int rstart = result.length - 1;
        int* result = (this->size < resultLen) ? (int*)malloc(sizeof(int)*resultLen): this->value;
         int resultlength = (this->size < resultLen)?resultLen:this->size;
         int rstart = resultlength - 1;
        unsigned long sum;
        long carry = 0;

        // Add common parts of both numbers
        while (x > 0 && y > 0) {
            x--;
            y--;
            int bval = y + addend.offset < addend.size ? addend.value[y
                    + addend.offset]
                    : 0;
            sum = (this->value[x + this->offset] & LONG_MASK) + (bval & LONG_MASK)
                    + carry;
            result[rstart--] = (int) sum;
            carry = sum >> 32;
        }

        // Add remainder of the longer number
        while (x > 0) {
            x--;
            if (carry == 0 && result == this->value && rstart == (x + this->offset)) {
                return;
            }
            sum = (this->value[x + this->offset] & LONG_MASK) + carry;
            result[rstart--] = (int) sum;
            carry = sum >> 32;
        }
        while (y > 0) {
            y--;
            int bval = y + addend.offset < addend.size ? addend.value[y
                    + addend.offset]
                    : 0;
            sum = (bval & LONG_MASK) + carry;
            result[rstart--] = (int) sum;
            carry = sum >> 32;
        }

        if (carry > 0) { // Result must grow in length
            resultLen++;
            if (resultlength < resultLen) {
                int* temp = (int*) malloc(sizeof(int)*resultLen);
                // Result one word longer from carry-out; copy low-order
                // bits into new result.
                arraycopy(result, 0, temp, 1, resultlength);
                temp[0] = 1;
                result = temp;
            } else {
                result[rstart--] = 1;
            }
        }

        this->value = result;
        this->size = resultlength;
        this->intLen = resultLen;
        this->offset = resultlength - resultLen;
    }

int compare(MutableBigInteger* this,MutableBigInteger b) {
        int blen = b.intLen;
        if (this->intLen < blen)
            return -1;
        if (this->intLen > blen)
            return 1;

        // Add Integer.MIN_VALUE to make the comparison act as unsigned integer
        // comparison.
        int* bval = b.value;
        int i,j,limit;
        i = this->offset;
        limit = this->intLen + this->offset;
        for (j = b.offset; i < limit; i++, j++) {
            int b1 = this->value[i] + 0x80000000;
            int b2 = bval[j] + 0x80000000;
            if (b1 < b2)
                return -1;
            if (b1 > b2)
                return 1;
        }
        return 0;
    }
void clear(MutableBigInteger* this) {
        this->offset = this->intLen = 0;
        int index,n;
        for (index = 0, n = this->size; index < n; index++)
            this->value[index] = 0;
    }
int numberOfLeadingZeros(int i) {
        // HD, Figure 5-6
        if (i == 0)
            return 32;
        int n = 1;
        if ((unsigned int)i >> 16 == 0) {
            n += 16;
            i <<= 16;
        }
        if ((unsigned int)i >> 24 == 0) {
            n += 8;
            i <<= 8;
        }
        if ((unsigned int)i >> 28 == 0) {
            n += 4;
            i <<= 4;
        }
        if ((unsigned int)i >> 30 == 0) {
            n += 2;
            i <<= 2;
        }
        n -= (unsigned int)i >> 31;
        return n;
    }
long divWord(long n, int d) {
        long dLong = d & LONG_MASK;
        long r;
        long q;
        if (dLong == 1) {
            q = (int) n;
            r = 0;
            return (r << 32) | (q & LONG_MASK);
        }

        // Approximate the quotient and remainder
        q = ((unsigned int)n >> 1) / ((unsigned int)dLong >> 1);
        r = n - q * dLong;

        // Correct the approximation
        while (r < 0) {
            r += dLong;
            q--;
        }
        while (r >= dLong) {
            r -= dLong;
            q++;
        }
        // n - q*dlong == r && 0 <= r <dLong, hence we're done.
        return (r << 32) | (q & LONG_MASK);
    }
void normalize(MutableBigInteger* this) {
        if (this->intLen == 0) {
            this->offset = 0;
            return;
        }

        int index = this->offset;
        if (this->value[index] != 0)
            return;

        int indexBound = index + this->intLen;
        do {
            index++;
        } while (index < indexBound && this->value[index] == 0);

        int numZeros = index - this->offset;
        this->intLen -= numZeros;
        this->offset = (this->intLen == 0 ? 0 : this->offset + numZeros);
    }
int getLowestSetBit(MutableBigInteger* this) {
        if (this->intLen == 0)
            return -1;
        int j, b;
        for (j = this->intLen - 1; (j > 0) && (this->value[j + this->offset] == 0); j--)
            ;
        b = this->value[j + this->offset];
        if (b == 0)
            return -1;
        return ((this->intLen - 1 - j) << 5) + numberOfTrailingZeros(b);
    }

void copyAndShift(int* src, int srcFrom, int srcLen,int* dst, int dstFrom, int shift) {
        int n2 = 32 - shift;
        int c = src[srcFrom];
        int i;
        for (i = 0; i < srcLen - 1; i++) {
            int b = c;
            c = src[++srcFrom];
            dst[dstFrom + i] = (b << shift) | ((unsigned int)c >> n2);
        }
        dst[dstFrom + srcLen - 1] = c << shift;
    }
int unsignedLongCompare(long long int one, long long int two) {
    long long int a1 =one -9223372036854775808;
    long long int a2 =two -9223372036854775808;
     return a1 > a2;
    }
int mulsubBorrow(int* q, int* a, int x, int len, int offset) {
        long long int xLong = x & LONG_MASK;
        long long int carry = 0;
        offset += len;
        int j;
        for (j = len - 1; j >= 0; j--) {
            long long int product = (a[j] & LONG_MASK) * xLong + carry;
            long long int difference = q[offset--] - product;
            carry = ((unsigned int)product >> 32)
                    + (((difference & LONG_MASK) > (((~(int) product) & LONG_MASK))) ? 1
                            : 0);
        }
        return (int) carry;
    }

void addDisjoint(MutableBigInteger* this,MutableBigInteger addend, int n) {
        if (isZero(addend))
            return;

        int x = this->intLen;
        int y = addend.intLen + n;
        int resultLen = (this->intLen > y ? this->intLen : y);
        int* result;
        int resultlength;
        if (this->size < resultLen){
            result = (int*)malloc(sizeof(int)*resultLen);
            resultlength = resultLen;
        }
        else {
            result = this->value;
            resultlength = this->size;
            fill(this->value, this->offset + this->intLen, this->size, 0);
        }

        int rstart = resultlength - 1;

        // copy from this if needed
        arraycopy(this->value, this->offset, result, rstart + 1 - x, x);
        y -= x;
        rstart -= x;

        int len = (y> addend.size - addend.offset)?addend.size - addend.offset:y;
        arraycopy(addend.value, addend.offset, result, rstart + 1
                - y, len);

        // zero the gap
        int i;
        for (i = rstart + 1 - y + len; i < rstart + 1; i++)
            result[i] = 0;

        this->value = result;
        this->size = resultlength;
        this->intLen = resultLen;
        this->offset = resultlength - resultLen;
    }

void primitiveLeftShift(MutableBigInteger* this,int n) {
        int* val = this->value;
        int n2 = 32 - n;
        int i,c,m;
        for ( i = this->offset, c = val[i], m = i + this->intLen - 1; i < m; i++) {
            int b = c;
            c = val[i + 1];
            val[i] = (b << n) | ((unsigned int)c >> n2);
        }
        val[this->offset + this->intLen - 1] <<= n;
    }
void primitiveRightShift(MutableBigInteger* this,int n) {
        int* val = this->value;
        int n2 = 32 - n;
        int i,c;
        for (i = this->offset + this->intLen - 1, c = val[i]; i > this->offset; i--) {
            int b = c;
            c = val[i - 1];
            val[i] = (c << n2) | ((unsigned int)b >> n);
        }
        val[this->offset]=(unsigned int)val[this->offset] >> n;
    }

void setValue(MutableBigInteger* this,int* val, int size,int length) {
        this->value = val;
        this->size = size;
        this->intLen = length;
        this->offset = 0;
    }

void safeLeftShift(MutableBigInteger* this,int n) {
        if (n > 0) {
            leftShift(this,n);
        }
    }

    /**
     * Left shift this MutableBigInteger n bits.
     */
void leftShift(MutableBigInteger* this,int n) {
        /*
         * If there is enough storage space in this MutableBigInteger already
         * the available space will be used. Space to the right of the used
         * ints in the value array is faster to utilize, so the extra space
         * will be taken from the right if possible.
         */
        if (this->intLen == 0)
            return;
        int nInts = n >> 5;
        int nBits = n & 0x1F;
        int bitsInHighWord = bitLengthForInt(this->value[this->offset]);

        // If shift can be done without moving words, do so
        if (n <= (32 - bitsInHighWord)) {
            primitiveLeftShift(this,nBits);
            return;
        }

        int newLen = this->intLen + nInts + 1;
        if (nBits <= (32 - bitsInHighWord))
            newLen--;
        if (this->size < newLen) {
            // The array must grow
            int* result =(int*)malloc(sizeof(int)*newLen);
            int i;
            for ( i = 0; i < this->intLen; i++)
                result[i] = this->value[this->offset + i];
            setValue(this,result,newLen, newLen);
        } else if (this->size - this->offset >= newLen) {
            // Use space on right
            int i;
            for (i = 0; i < newLen - this->intLen; i++)
                this->value[this->offset + this->intLen + i] = 0;
        } else {
            // Must use space on left
            int i;
            for (i = 0; i <  this->intLen; i++)
                this->value[i] = this->value[this->offset + i];
            for ( i = this->intLen; i < newLen; i++)
                this->value[i] = 0;
            this->offset = 0;
        }
        this->intLen = newLen;
        if (nBits == 0)
            return;
        if (nBits <= (32 - bitsInHighWord))
            primitiveLeftShift(this,nBits);
        else
            primitiveRightShift(this,32 - nBits);
    }

int bitCount(int i) {
        // HD, Figure 5-2
        i = i - (((unsigned int)i >> 1) & 0x55555555);
        i = (i & 0x33333333) + (((unsigned int)i >> 2) & 0x33333333);
        i = (i + ((unsigned int)i >> 4)) & 0x0f0f0f0f;
        i = i + ((unsigned int)i >> 8);
        i = i + ((unsigned int)i >> 16);
        return i & 0x3f;
    }
int bitLengthForInt(int n) {
        return 32 - numberOfLeadingZeros(n);
    }

int bitLength() {

  /*      int n = bitLength - 1;
        if (n == -1) { // bitLength not initialized yet
            int[] m = mag;
            int len = m.length;
            if (len == 0) {
                n = 0; // offset by one to initialize
            } else {
                // Calculate the bit length of the magnitude
                int magBitLength = ((len - 1) << 5)
                        + bitLengthForInt(mag[0]);
                if (signum < 0) {
                    // Check if magnitude is a power of two
                    int pow2 = (Integer.bitCount(mag[0]) == 1);
                    for (int i = 1; i < len && pow2; i++)
                        pow2 = (mag[i] == 0);

                    n = (pow2 ? magBitLength - 1 : magBitLength);
                } else {
                    n = magBitLength;
                }
            }
            bitLength = n + 1;
        }
        return n;*/
        return -9999999;
    }
int mulsub(int* q, int* a, int x, int len, int offset) {
        long long int xLong = x & LONG_MASK;
        long long int carry = 0;
        offset += len;
        int j;
        for (j = len - 1; j >= 0; j--) {
            long long int product = (a[j] & LONG_MASK) * xLong + carry;
            long long int difference = q[offset] - product;
            q[offset--] = (int) difference;
            carry = ((unsigned int)product >> 32)
                    + (((difference & LONG_MASK) > (((~(int) product) & LONG_MASK))) ? 1
                            : 0);
        }
        return (int) carry;
    }
int divadd(int* a,int asize, int* result, int offset) {
        long long int carry = 0;
        int j;
        for (j = asize - 1; j >= 0; j--) {
            long long int sum = (a[j] & LONG_MASK)
                    + (result[j + offset] & LONG_MASK) + carry;
            result[j + offset] = (int) sum;
            carry = (unsigned int)sum >> 32;
        }
        return (int) carry;
    }
MutableBigInteger divideMagnitude(MutableBigInteger* this,MutableBigInteger div,MutableBigInteger quotient, int needRemainder) {
        // assert div.intLen > 1
        // D1 normalize the divisor
        int shift = numberOfLeadingZeros(div.value[div.offset]);
        // Copy divisor value to protect divisor
        int dlen = div.intLen;
        int* divisor;
        MutableBigInteger rem; // Remainder starts as dividend with space for a leading zero
        if (shift > 0) {
            divisor = (int*)malloc(sizeof(int)*dlen);
            copyAndShift(div.value, div.offset, dlen, divisor, 0, shift);
            if (numberOfLeadingZeros(this->value[this->offset]) >= shift) {
                int* remarr = (int*)malloc(sizeof(int)*(this->intLen+1));;
                //rem = new MutableBigInteger(remarr);//p
                rem.intLen = this->intLen;
                rem.offset = 1;
                copyAndShift(this->value, this->offset, this->intLen, remarr, 1, shift);
            } else {
                int* remarr = (int*)malloc(sizeof(int)*(this->intLen+2));
                //rem = new MutableBigInteger(remarr);//p
                rem.intLen = this->intLen + 1;
                rem.offset = 1;
                int rFrom = this->offset;
                int c = 0;
                int n2 = 32 - shift;
                int i;
                for ( i = 1; i < this->intLen + 1; i++, rFrom++) {
                    int b = c;
                    c = this->value[rFrom];
                    remarr[i] = (b << shift) | ((unsigned int)c >> n2);
                }
                remarr[this->intLen + 1] = c << shift;
            }
        } else {
            copyOfRange(divisor,div.value, div.offset, div.offset
                    + div.intLen);
            //rem = new MutableBigInteger(new int[intLen + 1]);//p
            arraycopy(this->value, this->offset, rem.value, 1, this->intLen);
            rem.intLen = this->intLen;
            rem.offset = 1;
        }

        int nlen = rem.intLen;

        // Set the quotient size
        int limit = nlen - dlen + 1;
        if (quotient.size < limit) {
            quotient.value = (int*) malloc(sizeof(int)*limit);
            quotient.size = limit;
            quotient.offset = 0;
        }
        quotient.intLen = limit;
        int* q = quotient.value;

        // Must insert leading 0 in rem if its length did not change
        if (rem.intLen == nlen) {
            rem.offset = 0;
            rem.value[0] = 0;
            rem.intLen++;
        }

        int dh = divisor[0];
        long dhLong = dh & LONG_MASK;
        int dl = divisor[1];

        // D2 Initialize j
        int j;
        for ( j = 0; j < limit - 1; j++) {
            // D3 Calculate qhat
            // estimate qhat
            int qhat = 0;
            int qrem = 0;
            int skipCorrection = 0;
            int nh = rem.value[j + rem.offset];
            int nh2 = nh + 0x80000000;
            int nm = rem.value[j + 1 + rem.offset];

            if (nh == dh) {
                qhat = ~0;
                qrem = nh + nm;
                skipCorrection = qrem + 0x80000000 < nh2;
            } else {
                long nChunk = (((long) nh) << 32) | (nm & LONG_MASK);
                if (nChunk >= 0) {
                    qhat = (int) (nChunk / dhLong);
                    qrem = (int) (nChunk - (qhat * dhLong));
                } else {
                    long tmp = divWord(nChunk, dh);
                    qhat = (int) (tmp & LONG_MASK);
                    qrem = (int) ((unsigned int)tmp >> 32);
                }
            }

            if (qhat == 0)
                continue;

            if (!skipCorrection) { // Correct qhat
                long nl = rem.value[j + 2 + rem.offset] & LONG_MASK;
                long rs = ((qrem & LONG_MASK) << 32) | nl;
                long estProduct = (dl & LONG_MASK) * (qhat & LONG_MASK);

                if (unsignedLongCompare(estProduct, rs)) {
                    qhat--;
                    qrem = (int) ((qrem & LONG_MASK) + dhLong);
                    if ((qrem & LONG_MASK) >= dhLong) {
                        estProduct -= (dl & LONG_MASK);
                        rs = ((qrem & LONG_MASK) << 32) | nl;
                        if (unsignedLongCompare(estProduct, rs))
                            qhat--;
                    }
                }
            }

            // D4 Multiply and subtract
            rem.value[j + rem.offset] = 0;
            int borrow = mulsub(rem.value, divisor, qhat, dlen, j
                    + rem.offset);

            // D5 Test remainder
            if (borrow + 0x80000000 > nh2) {
                // D6 Add back
                divadd(divisor, rem.value,rem.size, j + 1 + rem.offset);
                qhat--;
            }

            // Store the quotient digit
            q[j] = qhat;
        } // D7 loop on j
          // D3 Calculate qhat
          // estimate qhat
        int qhat = 0;
        int qrem = 0;
        int skipCorrection = 0;
        int nh = rem.value[limit - 1 + rem.offset];
        int nh2 = nh + 0x80000000;
        int nm = rem.value[limit + rem.offset];

        if (nh == dh) {
            qhat = ~0;
            qrem = nh + nm;
            skipCorrection = qrem + 0x80000000 < nh2;
        } else {
            long nChunk = (((long) nh) << 32) | (nm & LONG_MASK);
            if (nChunk >= 0) {
                qhat = (int) (nChunk / dhLong);
                qrem = (int) (nChunk - (qhat * dhLong));
            } else {
                long tmp = divWord(nChunk, dh);
                qhat = (int) (tmp & LONG_MASK);
                qrem = (int) ((unsigned int)tmp >> 32);
            }
        }
        if (qhat != 0) {
            if (!skipCorrection) { // Correct qhat
                long nl = rem.value[limit + 1 + rem.offset] & LONG_MASK;
                long rs = ((qrem & LONG_MASK) << 32) | nl;
                long estProduct = (dl & LONG_MASK) * (qhat & LONG_MASK);

                if (unsignedLongCompare(estProduct, rs)) {
                    qhat--;
                    qrem = (int) ((qrem & LONG_MASK) + dhLong);
                    if ((qrem & LONG_MASK) >= dhLong) {
                        estProduct -= (dl & LONG_MASK);
                        rs = ((qrem & LONG_MASK) << 32) | nl;
                        if (unsignedLongCompare(estProduct, rs))
                            qhat--;
                    }
                }
            }

            // D4 Multiply and subtract
            int borrow;
            rem.value[limit - 1 + rem.offset] = 0;
            if (needRemainder)
                borrow = mulsub(rem.value, divisor, qhat, dlen, limit - 1
                        + rem.offset);
            else
                borrow = mulsubBorrow(rem.value, divisor, qhat, dlen, limit
                        - 1 + rem.offset);

            // D5 Test remainder
            if (borrow + 0x80000000 > nh2) {
                // D6 Add back
                if (needRemainder)
                    divadd(divisor, rem.value,rem.size, limit - 1 + 1 + rem.offset);
                qhat--;
            }

            // Store the quotient digit
            q[(limit - 1)] = qhat;
        }

        if (needRemainder) {
            // D8 Unnormalize
            if (shift > 0)
                rightShift(&rem,shift);
            normalize(&rem);
        }
        normalize(&quotient);
        if(needRemainder==1)return rem;
}

void rightShift(MutableBigInteger *this,int n) {
        if (this->intLen == 0)
            return;
        int nInts = (unsigned int)n >> 5;
        int nBits = n & 0x1F;
        this->intLen -= nInts;
        if (nBits == 0)
            return;
        int bitsInHighWord = bitLengthForInt(this->value[this->offset]);
        if (nBits >= bitsInHighWord) {
            primitiveLeftShift(this,32 - nBits);
            this->intLen--;
        } else {
            primitiveRightShift(this,nBits);
        }
    }
 void reset(MutableBigInteger *this) {
        this->offset = this->intLen = 0;
    }

void safeRightShift(MutableBigInteger *this,int n) {
        if (n / 32 >= this->intLen) {
            reset(this);
        } else {
            rightShift(this,n);
        }
    }

void keepLower(MutableBigInteger *this,int n) {
        if (this->intLen >= n) {
            this->offset += this->intLen - n;
            this->intLen = n;
        }
    }
int compareShifted(MutableBigInteger* this,MutableBigInteger b, int ints) {
        int blen = b.intLen;
        int alen = this->intLen - ints;
        if (alen < blen)
            return -1;
        if (alen > blen)
            return 1;

        // Add Integer.MIN_VALUE to make the comparison act as unsigned integer
        // comparison.
        int* bval = b.value;
        int i,j;
        for ( i = this->offset, j = b.offset; i < alen + this->offset; i++, j++) {
            int b1 = this->value[i] + 0x80000000;
            int b2 = bval[j] + 0x80000000;
            if (b1 < b2)
                return -1;
            if (b1 > b2)
                return 1;
        }
        return 0;
    }
void fill(int* array,int value,int size){
    int i;
    for(i=0;i<size;i++){
        array[i]=value;
    }
    return;
}
void ones(MutableBigInteger* this,int n) {
        if (n > this->size)
            this->value = (int*)malloc(sizeof(int)*n);
        fill(this->value, -1,(n > this->size)?n:this->size);
        this->offset = 0;
        this->intLen = n;
    }

int subtract(MutableBigInteger* this,MutableBigInteger b) {
        MutableBigInteger* a = this;

        int* result = this->value;
        int resultlength = this->size;
        int sign = compare(a,b);

        if (sign == 0) {
            reset(this);
            return 0;
        }
        if (sign < 0) {
            //MutableBigInteger* tmp = a;//p
            //a = &b;
            //&b = tmp;
        }

        int resultLen = a->intLen;
        if (resultlength < resultLen)
           {
               result = (int*)malloc(sizeof(int)*resultLen);
               resultlength = resultLen;
           }

        long long int diff = 0;
        int x = a->intLen;
        int y = b.intLen;
        int rstart = resultlength - 1;

        // Subtract common parts of both numbers
        while (y > 0) {
            x--;
            y--;

            diff = (a->value[x + a->offset] & LONG_MASK)
                    - (b.value[y + b.offset] & LONG_MASK)
                    - ((int) -(diff >> 32));
            result[rstart--] = (int) diff;
        }
        // Subtract remainder of longer number
        while (x > 0) {
            x--;
            diff = (a->value[x + a->offset] & LONG_MASK)
                    - ((int) -(diff >> 32));
            result[rstart--] = (int) diff;
        }

        this->value = result;
        this->size =resultlength;
        this->intLen = resultLen;
        this->offset = resultlength - resultLen;
        normalize(this);
        return sign;
    }

int divideOneWord(MutableBigInteger* this,int divisor, MutableBigInteger quotient) {
        long divisorLong = divisor & LONG_MASK;

        // Special case of one word dividend
        if (this->intLen == 1) {
            long long int dividendValue = this->value[this->offset] & LONG_MASK;
            int q = (int) (dividendValue / divisorLong);
            int r = (int) (dividendValue - q * divisorLong);
            quotient.value[0] = q;
            quotient.intLen = (q == 0) ? 0 : 1;
            quotient.offset = 0;
            return r;
        }

        if (quotient.size < this->intLen)
            {
                quotient.value = (int*) malloc(sizeof(int)*this->intLen);
                quotient.size = this->intLen;
            }
        quotient.offset = 0;
        quotient.intLen = this->intLen;

        // Normalize the divisor
        int shift = numberOfLeadingZeros(divisor);

        int rem = this->value[this->offset];
        long remLong = rem & LONG_MASK;
        if (remLong < divisorLong) {
            quotient.value[0] = 0;
        } else {
            quotient.value[0] = (int) (remLong / divisorLong);
            rem = (int) (remLong - (quotient.value[0] * divisorLong));
            remLong = rem & LONG_MASK;
        }
        int xlen = this->intLen;
        while (--xlen > 0) {
            long dividendEstimate = (remLong << 32)
                    | (this->value[this->offset + this->intLen - xlen] & LONG_MASK);
            int q;
            if (dividendEstimate >= 0) {
                q = (int) (dividendEstimate / divisorLong);
                rem = (int) (dividendEstimate - q * divisorLong);
            } else {
                long tmp = divWord(dividendEstimate, divisor);
                q = (int) (tmp & LONG_MASK);
                rem = (int) ((unsigned int)tmp >> 32);
            }
            quotient.value[this->intLen - xlen] = q;
            remLong = rem & LONG_MASK;
        }

        normalize(&quotient);
        // Unnormalize
        if (shift > 0)
            return rem % divisor;
        else
            return rem;
    }
MutableBigInteger divide(MutableBigInteger this,MutableBigInteger b,MutableBigInteger quotient, int needRemainder) {
        if (b.intLen < BURNIKEL_ZIEGLER_THRESHOLD
                || this.intLen - b.intLen < BURNIKEL_ZIEGLER_OFFSET) {
            return divideKnuth(this,b, quotient, needRemainder);
        } else {
            return divideAndRemainderBurnikelZiegler(this,b, quotient);
        }
    }
/**
     * Calculates the quotient of this div b and places the quotient in the
     * provided MutableBigInteger objects and the remainder object is returned.
     *
     * Uses Algorithm D in Knuth section 4.3.1.
     * Many optimizations to that algorithm have been adapted from the Colin
     * Plumb C library.
     * It special cases one word divisors for speed. The content of b is not
     * changed.
     *
     */
MutableBigInteger divideKnuth(MutableBigInteger this,MutableBigInteger b,MutableBigInteger quotient, int needRemainder) {


        // Dividend is zero
        if (this.intLen == 0) {
            quotient.intLen = quotient.offset = 0;
            //return needRemainder ? new MutableBigInteger() : null;//p
        }

        int cmp = compare(&this,b);
        // Dividend less than divisor
        if (cmp < 0) {
            quotient.intLen = quotient.offset = 0;
            //return needRemainder ? new MutableBigInteger(this) : null;//p
        }
        // Dividend equal to divisor
        if (cmp == 0) {
            quotient.value[0] = quotient.intLen = 1;
            quotient.offset = 0;
           // return needRemainder ? new MutableBigInteger() : null;//p
        }

        clear(&quotient);
        // Special case one word divisor
        if (b.intLen == 1) {
            int r = divideOneWord(&this,b.value[b.offset], quotient);
            if (needRemainder) {
               // if (r == 0)//p
                 //   return new MutableBigInteger();//p
               // return new MutableBigInteger(r);//p
            } //else {//p
               // return NULL;//p
            //}
        }

        // Cancel common powers of two if we're above the KNUTH_POW2_* thresholds
        if (this.intLen >= KNUTH_POW2_THRESH_LEN) {
            int trailingZeroBits = (getLowestSetBit(&this),
                    getLowestSetBit(&b))?getLowestSetBit(&this):getLowestSetBit(&b);
            if (trailingZeroBits >= KNUTH_POW2_THRESH_ZEROS * 32) {
                MutableBigInteger a;// = new MutableBigInteger(this);//p
               // b = new MutableBigInteger(b);//p
                rightShift(&a,trailingZeroBits);
                rightShift(&b,trailingZeroBits);
                MutableBigInteger r = divideKnuth(a,b, quotient,needRemainder);
                leftShift(&r,trailingZeroBits);
                return r;
            }
        }

        return divideMagnitude(&this,b, quotient, needRemainder);
    }
void copyOfRange(int* des,int* src,int start,int end){
                 //val.value, val.offset, val.offset+ intLen
    int i;
    int len = end-start;
    for(i=0;i<len;i++){
        des[i] = src[start+i];
    }
}
    /**
     * Computes {@code this/b} and {@code this%b} using the
     * <a href="http://cr.yp.to/bib/1998/burnikel.ps"> Burnikel-Ziegler algorithm</a>.
     * This method implements algorithm 3 from pg. 9 of the Burnikel-Ziegler paper.
     * The parameter beta was chosen to b 2<sup>32</sup> so almost all shifts are
     * multiples of 32 bits.<br/>
     * {@code this} and {@code b} must be nonnegative.
     * @param b the divisor
     * @param quotient output parameter for {@code this/b}
     * @return the remainder
     */
MutableBigInteger divideAndRemainderBurnikelZiegler(MutableBigInteger this,MutableBigInteger b, MutableBigInteger quotient) {
        int r = this.intLen;
        int s = b.intLen;

        // Clear the quotient
        quotient.offset = quotient.intLen = 0;

        if (r < s) {
            return this;
        } else {
            // Unlike Knuth division, we don't check for common powers of two here because
            // BZ already runs faster if both numbers contain powers of two and cancelling them has no
            // additional benefit.

            // step 1: let m = min{2^k | (2^k)*BURNIKEL_ZIEGLER_THRESHOLD > s}
            int m = 1 << (32 - numberOfLeadingZeros(s
                    / BURNIKEL_ZIEGLER_THRESHOLD));

            int j = (s + m - 1) / m; // step 2a: j = ceil(s/m)
            int n = j * m; // step 2b: block length in 32-bit units
            long n32 = 32L * n; // block length in bits
            long temp = n32 - bitLength(&b);
            int sigma = (int)(0, temp)?0:temp; // step 3: sigma = max{T | (2^T)*B < beta^n}//c
            MutableBigInteger bShifted;
            copyOfRange(&bShifted.value,&b.value,b.offset, b.offset+ b.intLen);
            bShifted.intLen = b.intLen ;

            safeLeftShift(&bShifted,sigma); // step 4a: shift b so its length is a multiple of n
            safeLeftShift(&this,sigma); // step 4b: shift this by the same amount

            // step 5: t is the number of blocks needed to accommodate this plus one additional bit
            int t = (int) ((bitLength() + n32) / n32);
            if (t < 2) {
                t = 2;
            }

            // step 6: conceptually split this into blocks a[t-1], ..., a[0]
            MutableBigInteger a1 = getBlock(t - 1, t, n); // the most significant block of this

            // step 7: z[t-2] = [a[t-1], a[t-2]]
            MutableBigInteger z = getBlock(t - 2, t, n); // the second to most significant block
            addDisjoint(&z,a1, n); // z[t-2]

            // do schoolbook division on blocks, dividing 2-block numbers by 1-block numbers
            MutableBigInteger qi;// = new MutableBigInteger();//p
            MutableBigInteger ri;
            int i;
            for (i = t - 2; i > 0; i--) {
                // step 8a: compute (qi,ri) such that z=b*qi+ri
                ri = divide2n1n(&z,bShifted, qi);

                // step 8b: z = [ri, a[i-1]]
                z = getBlock(i - 1, t, n); // a[i-1]
                addDisjoint(&z,ri, n);
                addShifted(&quotient,qi, i * n); // update q (part of step 9)
            }
            // iteration of step 8: do the loop one more time for i=0 but leave z unchanged
            ri = divide2n1n(&z,bShifted, qi);
            add(&quotient,qi);

            rightShift(&ri,sigma); // step 9: this and b were shifted, so shift back
            return ri;
        }
    }

    /**
     * This method implements algorithm 1 from pg. 4 of the Burnikel-Ziegler paper.
     * It divides a 2n-digit number by a n-digit number.<br/>
     * The parameter beta is 2<sup>32</sup> so all shifts are multiples of 32 bits.
     * <br/>
     * {@code this} must be a nonnegative number such that {@code this.bitLength() <= 2*b.bitLength()}
     * @param b a positive number such that {@code b.bitLength()} is even
     * @param quotient output parameter for {@code this/b}
     * @return {@code this%b}
     */
MutableBigInteger divide2n1n(MutableBigInteger* this,MutableBigInteger b,MutableBigInteger quotient) {
        int n = b.intLen;

        // step 1: base case
        if (n % 2 != 0 || n < BURNIKEL_ZIEGLER_THRESHOLD) {
            return divideKnuth(*this,b, quotient,1);
        }

        // step 2: view this as [a1,a2,a3,a4] where each ai is n/2 ints or less
        MutableBigInteger aUpper;// = new MutableBigInteger(this);//p
        safeRightShift(&aUpper,32 * (n / 2)); // aUpper = [a1,a2,a3]
        keepLower(this,n / 2); // this = a4

        // step 3: q1=aUpper/b, r1=aUpper%b
        MutableBigInteger q1; //= new MutableBigInteger();//p
        MutableBigInteger r1 = divide3n2n(&aUpper,b, q1);

        // step 4: quotient=[r1,this]/b, r2=[r1,this]%b
        addDisjoint(this,r1, n / 2); // this = [r1,this]
        MutableBigInteger r2 = divide3n2n(this,b, quotient);

        // step 5: let quotient=[q1,quotient] and return r2
        addDisjoint(&quotient,q1, n / 2);
        return r2;
    }

    /**
     * This method implements algorithm 2 from pg. 5 of the Burnikel-Ziegler paper.
     * It divides a 3n-digit number by a 2n-digit number.<br/>
     * The parameter beta is 2<sup>32</sup> so all shifts are multiples of 32 bits.<br/>
     * <br/>
     * {@code this} must be a nonnegative number such that {@code 2*this.bitLength() <= 3*b.bitLength()}
     * @param quotient output parameter for {@code this/b}
     * @return {@code this%b}
     */
MutableBigInteger divide3n2n(MutableBigInteger* this,MutableBigInteger b,MutableBigInteger quotient) {
        int n = b.intLen / 2; // half the length of b in ints

        // step 1: view this as [a1,a2,a3] where each ai is n ints or less; let a12=[a1,a2]
        MutableBigInteger a12 = new MutableBigInteger(this);
        a12.safeRightShift(32 * n);

        // step 2: view b as [b1,b2] where each bi is n ints or less
        MutableBigInteger b1 = new MutableBigInteger(b);
        b1.safeRightShift(n * 32);
        BigInteger b2 = b.getLower(n);

        MutableBigInteger r;
        MutableBigInteger d;
        if (compareShifted(b, n) < 0) {
            // step 3a: if a1<b1, let quotient=a12/b1 and r=a12%b1
            r = a12.divide2n1n(b1, quotient);

            // step 4: d=quotient*b2
            d = new MutableBigInteger(quotient.toBigInteger().multiply(b2));
        } else {
            // step 3b: if a1>=b1, let quotient=beta^n-1 and r=a12-b1*2^n+b1
            quotient.ones(n);
            a12.add(b1);
            b1.leftShift(32 * n);
            a12.subtract(b1);
            r = a12;

            // step 4: d=quotient*b2=(b2 << 32*n) - b2
            d = new MutableBigInteger(b2);
            d.leftShift(32 * n);
            d.subtract(new MutableBigInteger(b2));
        }

        // step 5: r = r*beta^n + a3 - d (paper says a4)
        // However, don't subtract d until after the while loop so r doesn't become negative
        r.leftShift(32 * n);
        r.addLower(this, n);

        // step 6: add b until r>=d
        while (r.compare(d) < 0) {
            r.add(b);
            quotient.subtract(MutableBigInteger.ONE);
        }
        r.subtract(d);

        return r;
    }

    /**
     * Returns a {@code MutableBigInteger} containing {@code blockLength} ints from
     * {@code this} number, starting at {@code index*blockLength}.<br/>
     * Used by Burnikel-Ziegler division.
     * @param index the block index
     * @param numBlocks the total number of blocks in {@code this} number
     * @param blockLength length of one block in units of 32 bits
     * @return
     */
MutableBigInteger getBlock(MutableBigInteger*this,int index, int numBlocks,int blockLength) {
        int blockStart = index * blockLength;
        if (blockStart >= this->intLen) {
            return new MutableBigInteger();
        }

        int blockEnd;
        if (index == numBlocks - 1) {
            blockEnd = this->intLen;
        } else {
            blockEnd = (index + 1) * blockLength;
        }
        if (blockEnd > this->intLen) {
            return new MutableBigInteger();
        }

        int[] newVal = Arrays.copyOfRange(this->value,
                this->offset + this->intLen - blockEnd, this->offset + this->intLen - blockStart);
        return new MutableBigInteger(newVal);
    }

    /** @see BigInteger#bitLength() */
long bitLength() {
        if (intLen == 0)
            return 0;
        return intLen * 32L - Integer.numberOfLeadingZeros(value[offset]);
    }
