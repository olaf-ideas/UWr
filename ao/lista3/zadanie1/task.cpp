#include <bits/stdc++.h>

using namespace std;

vector<pair<string, int>> roman = {
	{"M", 1000},
	{"CM", 900},
	{"D", 500},
	{"CD", 400},
	{"C", 100},
	{"XC", 90},
	{"L", 50},
	{"XL", 40},
	{"X", 10},
	{"IX", 9},
	{"V", 5},
	{"IV", 4},
	{"I", 1},
};

string to_roman(int n) {
	assert(1 <= n && n <= 3999);

	string result;
	for (auto [symbol, value] : roman) {
		while (n >= value) {
			n -= value;
			result += symbol;
		}
	}

	return result;
}

int main() {
	int n;
	cin >> n;

	cout << to_roman(n) << '\n';

	return 0;
}
