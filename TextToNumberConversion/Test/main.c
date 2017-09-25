#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define t(i) printf("%llu\n",i);
#define s(i) printf("%s\n",i);
#define f(digit,limit)  printf("-----------------------------\n");\
                        for(i=0; i<limit; i++){\
                            t(digit[i]);\
                        }\
                        printf("-----------------------------\n");\

typedef unsigned long long int LONG;
typedef unsigned char UCHAR;
LONG swap_uint64( LONG val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}

int EndianMachine(){

   unsigned int i = 1;
   char *c = (char*)&i;
   if (*c)
       return 1;
   else
       return 0;
}

void MakeLittleEndianFormat(LONG* data, int length){
    int i=0;
    for(i=0; i<length; i++){
        data[i] = swap_uint64(data[i]);
    }
}
void AddZeroPadding(UCHAR* msg, int size){
    strcpy(msg,"");
    if(size>=7)return;
    int i=0;
    for(i=0; i<size; i++){
        strcat(msg,"0");
    }
}
void ToChar(LONG ulong_value, UCHAR* msg){
    UCHAR OriginalMsg[1000];

    const int n = snprintf(NULL, 0, "%llu", ulong_value);
    int c = snprintf(OriginalMsg, n+1, "%llu", ulong_value);
    OriginalMsg[n]='\0';

    AddZeroPadding(msg,20-n);
    strcat(msg,OriginalMsg);


}
void CharToInteger(UCHAR* message, UCHAR* FinalResult){

    int i;
    int littleendian = EndianMachine();
    UCHAR *msg;
    int lenv = strlen(message)+1;
    msg = (UCHAR*)malloc(sizeof(UCHAR)*lenv);

    memset(msg,0,lenv);
    strcpy(msg,message);//"1234567810");
    int msglen = strlen(msg);
    int length = (int)ceil((double)msglen/sizeof(LONG));

    LONG* digit;
    digit = (LONG*)msg;
  //  f(digit,length);

    if(!littleendian){

        MakeLittleEndianFormat(digit,length);

    }

   // f(digit,length);

    strcpy(FinalResult,"");
    UCHAR subresult[30];

    for(i=0; i<length; i++){
            memset(subresult,0,30);
            ToChar(digit[i],subresult);
            //s(baal);
            strcat(FinalResult,subresult);
    }


    printf("%d %d %d",strlen(message),strlen(message)*3,strlen(FinalResult));
}

void IntegerToChar(UCHAR* number, UCHAR* msg){
    LONG* integerlist;
    UCHAR substrof20len[25];
    int numberlen = strlen(number);
    int listitem = (int)ceil((double)numberlen/20);
    integerlist = (LONG*)malloc((listitem+1)*sizeof(LONG));
    int i,j,k;
    for(i=0; i<listitem; i++){
        k=0;
        int limit = i*20+20;

        if(i==59){
            printf("yes");
        }
        if(i!=listitem-1){ //20

            for(j=limit-20;j<limit;j++){
                substrof20len[k++] = number[j];
            }
            substrof20len[k] = '\0';
        }
        else{
            for(j=limit-20;j<numberlen;j++){
                substrof20len[k++] = number[j];
            }
            substrof20len[k] = '\0';
        }

        integerlist[i] = (LONG)atoll(substrof20len);
        //printf("\n%s %llu\n",substrof20len, integerlist[i]);
    }
    integerlist[listitem]=0;

    msg = (UCHAR*)integerlist;

    s(msg);

}
int main()
{
    UCHAR FinalNumber[10000];
    UCHAR FinalMessage[10000];


    CharToInteger("ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ!@$@#EDFSDGERYEY*&^(*)(_+PIYKTHDFGdfhdfsghre0t934-63=609-508094yijkghfkdjghkfvjksdfhkÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ15253472674678382hdjhjksdvjhhcvnzxjkvjhsdfjhfjksdnjkfh1515253472674678382hdjhjksdvjhhcvnzxjkvjhsdfjhfjksdnjkfh152515253472674678382hdjhjksdvjhhcvnzxjkvjhsdfjhfjksdnjkfh15253472674615253472674678382hdjhjksdvjhhcvnzxjkvjhsdfjhfjksdnjkfh15253472674678382hdjhjksdvjhhcvnzxjkvj\nhsdfjhfjksdnjkfh\n15253472674678382hdjhjksdvjhhcvnzxjkvjhsdfjhfjksdnjkfh78382hdjhjksdvjhhcvnzxjkvj\nhsdfjhfjksdnjkfh\n15253472674678382hdjhjksdvjhhcvnzxjkvjhsdfjhfjksdnjkfh3472674678382hdjhjksdvjhhcvnzxjkvj\nhsdfjhfjksdnjkfh\n15253472674678382hdjhjksdvjhhcvnzxjkvjhsdfjhfjksdnjkfh253472674678382hdjhjksdvjhhcvnzxjkvj\nhsdfjhfjksdnjkfh\n15253472674678382hdjhjksdvjhhcvnzxjkvjhsdfjhfjksdnjkfh15253472674678382hdjhjksdvjhhcvnzxjkvjhsdfjhfjksdnjkfh15253472674678382hdjhjksdvjhhcvnzxjkvj\nhsdfjhfjksdnjkfh\n15253472674678382hdjhjksdvjhhcvnzxjkvjhsdfjhfjksdnjkfh15253472674678382hdjhjksdvjhhcvnzxjkvjhsdfjhfjksdnjkfh15253472674678382hdjhjksdvjhhcvnzxjkvj\nhsdfjhfjksdnjkfh\n15253472674678382hdjhjksdvjhhcvnzxjkvjhsdfjhfjksdnjkfh",FinalNumber);

   // printf("\n%s\n",FinalNumber);

    IntegerToChar(FinalNumber,FinalMessage);

    unsigned char a = 'ÿ';
    unsigned int b = a;


    return 0;
}
