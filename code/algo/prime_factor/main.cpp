#include <iostream>
#include <set>
#include <cmath>

using namespace std;

set<int> f(int n)
{

  set<int> s = {1};

  // Делим пока делится на 2
  while (n % 2 == 0)
  {
    s.insert(2);
    n = n / 2;
  }

  // Если нечётное, то пробегаемся от 3 (первый нечёт после 1) до корня (есть хотя бы одиг простой делитель составного числа который меньше или равен корню числа) от нечётного числа
  for (int i = 3; i <= sqrt(n); i = i + 2)
  {
    // Если делится на число из диапазона от 3 до нечётного числа, то продолжаем делить на него
    while (n % i == 0)
    {

      s.insert(i);
      n = n / i;
    }
  }

  // Остальное вставляем
  if (n > 2)
  {
    s.insert(n);
  }

  return s;
}

int main()
{

  int x;

  cin >> x;

  for (int i : f(x))
  {
    cout << i << " ";
  }

  return 0;
}