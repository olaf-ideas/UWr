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
	// Zadanie 1
	int n;
	cin >> n;

	cout << to_roman(n) << '\n';

	// Zadanie 2
	map<string, int> to_arabic;
	for (int i = 1; i <= 3999; i++)
		to_arabic[to_roman(i)] = i;

	string s;
	cin >> s;

	auto it = to_arabic.find(s);
	if (it == to_arabic.end())
		cout << "NIE\n";
	else
		cout << it->second << '\n';

	return 0;
}
