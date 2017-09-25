#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "fft.h"
#include "bignum.h"
#include "mul.h"
#include "constant.h"
#include "sub.h"
#include "sum.h"
#include "karatsubaMul.h"
#include "slowdiv.h"
#include "addsub.h"

char* publickey = "1000111011010100110111011101001111111000100001010101000101111111111010100001000001111101101101010001100001110110101100011000100111001";
char* privateKey = "11010111010011111000101001011101100001001110000000001000110111000111101111010101110111001001110010101100011000011111100011111000000001010100010111000010000111001010110001000101000011111111111001011000011000001111111101110000011000110010100111011111110000101001";

char* ERSAEncryption(char* m, char* e, char* n){
	//c=m^e mod n
	/*procedure modular exponentiation(m: integer, e = (ak−1ak−2 . . . a1a0)2,
	n: positive integers)
	x := 1
	power := m mod n
	for i := 0 to k − 1
	if ai = 1 then x := (x · power) mod n
	power := (power · power) mod n
	return x{x equals me mod n}*/
    char power[N_DIGIT]; //as m<n so no need to mod for rsa
    char result[N_DIGIT];
    strcpy(&power,m);
	strcpy(&result,"1");


    BigNumber Result ;
	BigNumber Product ;
	BigNumber N;
    BigNumber DIV;
	initialize(&Product,&power);
    initialize(&Result,&result);
    initialize(&N,n);
    initialize(&DIV,"");


    int wx=0,wax=0;
    int len = strlen(e);
    int count =0;
	int i = 0;
	for(i=len-1;i>=0;i--){

		if (e[i]=='1') {

            wx++;
			fastMul(&Result,&Product,&Result);
            if(wx%DIVIDE_THRESHOLD==0){
                slowdiv(&Result,&N,&DIV);
                slowmod(&Result,&N,&DIV);
                count++;
                copy(&DIV,&Result);
            }

		}
        wax++;
		fastMul(&Product,&Product,&Product);
        if(wax%DIVIDE_THRESHOLD==0){
            count++;
            slowdiv(&Product,&N,&DIV);
            slowmod(&Product,&N,&DIV);
            copy(&DIV,&Product);
        }

	}


    slowdiv(&Result,&N,&DIV);
    slowmod(&Result,&N,&DIV);
    count++;
    copy(&DIV,&Result);
   // printf("\n");
	//show(&Result,15000,9);
	//printf("\n");
	//printf("\ncount =%d\n",count);
	//printf("b==%s",X.digits);
	//printf("??%d??%s???",i, result);
	//printf("<<<%s ", m);
	//printf("%s >>>", result);
	/*int len = strlen(result);
	if (len % 2 == 1){
		strrev(result);
		strcat(result, "0");
		strrev(result);
	}
	strcat(result, "-");
	return result;*/
}

int main()
{

    const  char a1[4000]="";
	const  char a2[4000]="";
	const  char a3[4000]="";
	const  char a4[4000]="";
	//scanf("%s",a1);
	//scanf("%s",a2);
	//scanf("%s",a3);


    clock_t t;
    t = clock();
    int i;
    for(i=0;i<12;i++){
    BigNumber Result ;//= BigNumber(a1);
	BigNumber Product ;//= BigNumber(a2);
	BigNumber BX;
	//initialize(&X,a1);
    //initialize(&AX,a2);
    //initialize(&BX,a3);
      // fastMul(&X,&AX,&X);
      //karatsuba(a1,a2,a3);
    //    slowmod(&X,&AX,&BX);
      //slowdiv(&X,&AX,&X);

    //ERSAEncryption("7476767474747676747474767674747476767474747676747474767676747474767674747476",publickey,"3521851118865011044136429217528930691441965435121409905222808922963363310303627");

      ERSAEncryption("140936382229776887437600729318970213964398759211095959887920668800310339796006",privateKey,"3521851118865011044136429217528930691441965435121409905222808922963363310303627");
    }
  //  product(a1,a2);
	t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    printf("fun() took %f seconds to execute \n", time_taken);




    return 0;
}
