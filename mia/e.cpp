#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	#define int long long

	int n;
	cin >> n;

	map<int, int> A, B;
	vector<tuple<int, int, int>> rect(n);
	
	int g = 0;
	for (auto& [w, h, c] : rect) {
		cin >> w >> h >> c;
		A[w] += c;
		B[h] += c;
		g = gcd(g, c);
	}

	sort(rect.begin(), rect.end());

	int i = 0;
	while (i < n && get<0>(rect[i]) == get<0>(rect[0]))
		i++;
	
	if (n % i != 0) {
		cout << "0\n";
		return 0;
	}

	int j = i;
	while (j < n) {
		for (int k = 0; k < i; k++) {
			auto [w0, h0, c0] = rect[0];
			auto [wk, hk, ck] = rect[0 + k];
			auto [w1, h1, c1] = rect[j];
			auto [wj, hj, cj] = rect[j + k];

			if (c0 * cj == c1 * ck || w0 != wj || hk != hj) {
				cout << "0\n";
				return 0;
			}
		}
		
		j += i;
	}

	int ans = 0;
	for (int i = 1; i * i <= g; i++) {
		if (g % i == 0) {
			ans++;
			if (i * i != g)
				ans++;
		}
	}

	cout << ans << '\n';

	return 0;
}

