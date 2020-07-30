#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

int binarySearch(int vec[], int left, int right, int x)
{
	if (right >= left)
	{
		int mid = left + (right - left) / 2;

		if (vec[mid] == x)
		{
			return mid;
		}

		if (vec[mid] > x)
		{

			return binarySearch(vec, left, mid - 1, x);
		}

		return binarySearch(vec, mid + 1, right, x);
	}

	return -1;
}

int main()
{
	int first_len, second_len;
	cin >> first_len >> second_len;

	int first_arr[first_len] = {};
	int second_arr[second_len] = {};

	for (int i = 0; i < first_len; i++)
	{
		int x;
		cin >> x;
		first_arr[i] = x;
	}

	for (int i = 0; i < second_len; i++)
	{
		int x;
		cin >> x;
		second_arr[i] = x;
	}

	for (int second_el : second_arr)
	{
		int n = sizeof(second_arr) / sizeof(second_arr[0]);

		int result = binarySearch(first_arr, 0, n - 1, second_el);

		if (result != -1)
		{
			cout << "YES" << endl;
		}
		else
		{
			cout << "NO" << endl;
		}
	}

	return 0;
}