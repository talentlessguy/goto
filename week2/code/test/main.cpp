#include <iostream>

using namespace std;

int euclid(int a, int b)
{
  while (a != 0 && b != 0)
  {
    b = b % a;

    if (b == 0)
    {
      break;
    }

    a = a % b;
  }

  if (b > a)
  {
    return b;
  }
  else
  {
    return a;
  }
}

int main()
{
  int a = 28, b = 21;

  cout << euclid(a, b);
}