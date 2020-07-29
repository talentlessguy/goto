#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

struct BinVec {
	vector<int> vec;
	int num;
};

BinVec bin_search(BinVec args) {
	int middle = args.vec[args.vec.size() / 2];

	cout << "Middle " << middle << endl; 

	cout << "[";

	for (auto el : args.vec) {
		cout << el << ",";
	}

	cout <<  "]\n";

	if (args.vec.size() <= 3 || middle == args.num) {
	
		vector<int> vec;
		
		vec.push_back(middle);
		
		return { vec, args.num };
		
	} else if (middle > args.num) {
	
			vector<int> slice;

			for (auto i = args.vec.size() / 2 - 1; i < args.vec.size(); i++) {
				slice.push_back(i);
			}
			
			return bin_search({ slice, args.num });
	} else if (middle < args.num) {
			vector<int> slice;

				for (auto i = args.vec.size() / 2; i < args.vec.size(); i++) {
				slice.push_back(i);
			}
			
			return bin_search({ slice, args.num });
	}
};

int main() {

	// cin >> n >> k;

	vector<int> first_arr = { 1, 2, 3, 4, 5, 6, 8, 9 };
	// vector<int> second_arr;

	// for (int i = 0; i < k; i++) {
		// int x;
		// cin >> x;
		// second_arr.push_back(x);
	// }

	cout << "Find 4 " << endl;

	auto s = bin_search({first_arr, 4 });


	// for (i = 0; i < k; i++) {
		// 
	// }
};
