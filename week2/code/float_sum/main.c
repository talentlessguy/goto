#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

struct my_double
{
  unsigned expp : 11;
  unsigned sign : 1;
  unsigned long long mantissa : 52;
};

union unpacker {
  double fp_number;
  struct my_double inside;
};

int main()
{
  FILE *file;

  int c;

  union unpacker result;

  result.fp_number = 0;

  file = fopen("floats.txt", "r");

  if (file)
  {
    while ((c = getc(file)) != EOF)
    {
      union unpacker n;
      n.fp_number = c;

      result.inside.expp;
    }

    fclose(file);
  }

  printf("%d", result.fp_number);

  return 0;
}
