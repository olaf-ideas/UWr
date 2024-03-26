#include <bits/stdc++.h>

using namespace std;

const int K = 6;

int main() {

	vector<int> p;
	for (int i = 1; i <= K; i++) {
		int cnt;
		cin >> cnt;

		while (cnt--)
			p.push_back(i);
	}

	sort(p.begin(), p.end());

	int ans = 1e9;

	do {
		int res = 0;
		int now = 0;

		for (int x : p) {
			now += x;
			if (now > K) {
				res++;
				now = x;
			}
		}

		if (now > 0)
			res++;

		ans = min(ans, res);

	} while (next_permutation(p.begin(), p.end()));

	cout << ans << '\n';

	return 0;
}
