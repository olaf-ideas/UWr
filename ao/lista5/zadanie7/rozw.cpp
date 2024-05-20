#include <iostream>

using namespace std;

const int MAX_C = 300000;

int n, a, b;
int dp[MAX_C];

int main() {
	cin >> n >> a >> b;

	for (int i = 0; i < max(a, b); i++)
		dp[i] = 1;
	
	int cost = max(a, b);

	while (dp[cost - 1] < n) {
		dp[cost] = dp[cost - a] + dp[cost - b];
		cost++;
	}

	int l = 1, r = n;

	while (l < r) {
		while (dp[cost - 1] >= r - l + 1)
			cost--;

		cout << "N " << l + dp[cost - a] << endl;

		string res;
		cin >> res;

		if (res == "tak") {
			r = l + dp[cost - a] - 1;
			cost -= a;
		}
		else
		if (res == "nie") {
			l = l + dp[cost - a];
			cost -= b;
		}
	}

	cout << "! " << l << endl;

	return 0;
}
