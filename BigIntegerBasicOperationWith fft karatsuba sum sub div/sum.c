

void sum(char* a, char* b, char* res)
{
  int lena = strlen(a);
  int lenb = strlen(b);

  if (lena < lenb)
  {

    int temp;

    temp =a;
    a=b;
    b=temp;

    temp = lena;
    lena=lenb;
    lenb=temp;
  }

  int toAdd = 0;
  int inda,indb;
  for (inda = lena - 1, indb = lenb - 1; inda >= 0; --inda, --indb)
    {
        int x = a[inda] - '0';
        int y = indb >= 0 ? b[indb] - '0' : 0;

        int cur = x + y + toAdd;

        if (cur >= 10)
        {
          toAdd = 1;
          cur -= 10;
        }
        else
        {
          toAdd = 0;
        }

        res[inda] = cur + '0';
    }

  if (toAdd == 1)
  {
    add_zeros(res, 1);
    res[0] = '1';
  }
}
