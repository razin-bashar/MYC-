void sub(char* a, char* b, char* res)
{
  int lena = strlen(a);
  int lenb = strlen(b);

  //assert(lena >= lenb);

  int toSub = 0;
  int inda,indb;
  for (inda = lena - 1, indb = lenb - 1; inda >= 0; --inda, --indb)
  {
    int x = a[inda] - '0';
    int y = indb >= 0 ? b[indb] - '0' : 0;

    if (toSub == 1)
    {
      x--;
    }
    int cur;
    if (x < y)
    {
      cur = x + 10 - y;
      toSub = 1;
    }
   else
   {
      cur = x - y;
      toSub = 0;
   }

    res[inda] = cur + '0';
  }
}
