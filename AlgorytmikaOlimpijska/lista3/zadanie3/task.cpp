#include <bits/stdc++.h>

using namespace std;

const int K = 6;

vector<vector<int>> S;

vector<int> now;
void rek(int n, int last) {
	for (int j = min(n, last); j >= 1; j--) {
		now.push_back(j);
		rek(n - j, j);
		now.pop_back();
	}

	if (now.size())
		S.push_back(now);
}

int main() {
	ios::sync_with_stdio(false), cin.tie(nullptr);

	rek(K, K);

	/*
	for (auto v : S) {
		for (int x : v)
			cerr << x << ' ';
		cerr << '\n';
	}
	// */

	vector<int> cnt(K + 1);
	for (int i = 1; i <= K; i++)
		cin >> cnt[i];

	int ans = 0;
	for (auto v : S) {
		while (true) {
			vector<int> nxt = cnt;

			bool ok = true;
			for (int x : v)
				if (--nxt[x] < 0)
					ok = false;

			if (ok) {
				ans++;
				cnt = nxt;
			}
			else {
				break;
			}
		}
	}

	cout << ans << '\n';

	return 0;
}
