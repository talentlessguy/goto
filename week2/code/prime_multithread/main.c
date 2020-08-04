#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <math.h>

#define CHILDREN 4

int is_prime(int num)
{
  if (num <= 1)
    return 0;
  if (num % 2 == 0 && num > 2)
    return 0;
  for (int i = 3; i < (sqrt(num)); i += 2)
  {
    if (num % i == 0)
      return i;
  }
  return 0;
}

int main()
{

  int start = 1;
  int len = 9;

  int fds[2];

  pipe(fds);

  int result;

  int counter = 0;

  for (int i = 0; i < CHILDREN; ++i)
  {
    if (fork())
    {
      continue;
    }

    int sstart = start + (len / CHILDREN) * i;
    int ffinish = start + (len / CHILDREN) * (i + 1);

    for (int n = sstart; n < ffinish + 1; n++)
    {

      volatile int result = is_prime(n);

      write(fds[1], (void *)(&result), sizeof(int));
    }

    result = -1;
    write(fds[1], (void *)(&result), sizeof(int));

    return 0;
  }

  while (counter != CHILDREN)
  {

    read(fds[0], (void *)(&result), sizeof(int));

    if (result == -1)
    {
      counter++;
    }
    else
    {
      printf("%d result: %d\n", counter, result);
    }
  }
}
