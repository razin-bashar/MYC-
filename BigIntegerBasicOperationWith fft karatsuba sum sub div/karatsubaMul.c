void add_zeros(char* a, int n)
{
  int lena = strlen(a);
  int i;
  for (i = lena - 1 + n; i >= n; --i)
  {
    a[i] = a[i - n];
  }
  a[lena + n] = 0;
  for (i = 0; i < n; ++i)
  {
    a[i] = '0';
  }
}

void remove_zeros(char* a)
{
  int lena = strlen(a);
  int ind = 0;
  while (ind < lena && a[ind] == '0')
  {
    ++ind;
  }
  int i;
  for ( i = ind; i < lena; ++i)
  {
    a[i - ind] = a[i];
  }
  a[lena - ind] = 0;
}
void mult10(char* a, int n)
{
  int lena = strlen(a);
  if (lena == 1 && a[0] == '0')
  {
    return;
  }
  int i;
  for (i = lena; i < lena + n; ++i)
  {
    a[i] = '0';
  }
  a[lena + n] = 0;
}

char* CreateArray(int len)
{
  char* res = (char*)malloc(sizeof(char)*(len+1));
  memset(res, 0, len+1);
  return res;
}

// add leading zeros if needed
void make_equal_length(char* a, char* b)
{
  int lena = strlen(a);
  int lenb = strlen(b);
  int n = (lena>lenb)?lena:lenb;
  add_zeros(a, n - lena);
  add_zeros(b, n - lenb);
}

void karatsuba(char* x, char* y, char* res)
{
  make_equal_length(x, y);
  int len = strlen(x);
  if (len == 1)
    {
    int val = (x[0] - '0') * (y[0] - '0');
    if (val < 10) {
      res[0] = val + '0';
    }
  else
    {
      res[0] = (val / 10) + '0';
      res[1] = (val % 10) + '0';
    }
  }
  else
  {
    char* xl = CreateArray(len);
    char* xr = CreateArray(len);
    char* yl = CreateArray(len);
    char* yr = CreateArray(len);

    int rightSize = len / 2;
    int leftSize = len - rightSize;

    strncpy(xl, x, leftSize);
    strcpy(xr, x + leftSize);
    strncpy(yl, y, leftSize);
    strcpy(yr, y + leftSize);
    int maxl = 3 * len;
    char* P1 = CreateArray(maxl);
    char* P2 = CreateArray(maxl);
    char* P3 = CreateArray(maxl);

    karatsuba(xl, yl, P1);
    karatsuba(xr, yr, P2);

    char* tmp1 = CreateArray(maxl);
    char* tmp2 = CreateArray(maxl);
    sum(xl, xr, tmp1);
    sum(yl, yr, tmp2);
    karatsuba(tmp1, tmp2, P3);
    sub(P3, P1, P3);
    sub(P3, P2, P3);
    mult10(P3, rightSize);
    mult10(P1, 2 * rightSize);
    sum(P1, P2, res);
    sum(res, P3, res);
    remove_zeros(res);

    free(xl);
    free(xr);
    free(yl);
    free(yr);
    free(tmp1);
    free(tmp2);
    free(P1);
    free(P2);
    free(P3);
  }
}
