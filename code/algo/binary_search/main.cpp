#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

struct BinVec
{
	vector<int> vec;
	int num;
};

BinVec bin_search(BinVec args)
{

	vector<int> slice;

	int n = args.vec.size();
	int l = 0;
	int r = n - 1;
	int t = args.num;

	while (l <= r)
	{
		int m = floor((l + r) / 2);

		if (args.vec[m] < t)
		{
		}
	}

	return bin_search({slice, args.num});
};

int main()
{

	// cin >> n >> k;

	vector<int> first_arr = {1, 2, 3, 4, 5, 6, 8, 9, 10};
	// vector<int> second_arr;

	// for (int i = 0; i < k; i++) {
	// int x;
	// cin >> x;
	// second_arr.push_back(x);
	// }

	cout << "Find 6 " << endl;

	auto s = bin_search({first_arr, 6});

	cout << endl
			 << "Result " << s.vec[0];

	// for (i = 0; i < k; i++) {
	//
	// }
};
