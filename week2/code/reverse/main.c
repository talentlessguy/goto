#include <stdio.h>

int main()
{
  int nums[5] = {1, 2, 3, 4, 5};

  int arr_len = sizeof(nums) / sizeof(int);

  for (int i = arr_len - 1; i > -1; i--)
  {
    printf("%d", nums[i]);
  }
}