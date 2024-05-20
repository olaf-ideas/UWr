#include <bits/stdc++.h>

using namespace std;

const int K = 5;

int cnt[K + 1], ile;

int main() {
	ios::sync_with_stdio(false), cin.tie(nullptr);

	for (int i = 1; i <= K; i++)
		cin >> cnt[i];
	
	int ans = 0;
	
	// (5)
	ans += cnt[5];
	cnt[5] = 0;

	// (4, 1), (4)
	cnt[1] = max(0, cnt[1] - cnt[4]);
	ans += cnt[4];
	cnt[4] = 0;

	// (3, 2)
	ile = min(cnt[2], cnt[3]);
	ans += ile;
	cnt[3] -= ile;
	cnt[2] -= ile;

	// (3, 1, 1)
	ile = min(cnt[3], cnt[1] / 2);
	ans += ile;
	cnt[3] -= ile;
	cnt[1] -= ile * 2;

	// (3, 1)
	ile = min(cnt[3], cnt[1]);
	ans += ile;
	cnt[3] -= ile;
	cnt[1] -= ile;

	// (3)
	ans += cnt[3];
	cnt[3] = 0;

	// (2, 2, 1)
	ile = min(cnt[2] / 2, cnt[1]);
	ans += ile;
	cnt[2] -= ile * 2;
	cnt[1] -= ile;

	// (2, 2)
	ile = cnt[2] / 2;
	ans += ile;
	cnt[2] -= ile * 2;

	// (2, 1, 1, 1) / (2, 1, 1) / (2, 1)
	ile = min(cnt[2], (cnt[1] + 2) / 3);
	ans += ile;
	cnt[2] -= ile;
	cnt[1] = max(0, cnt[1] - ile * 3);

	// (2)
	ans += cnt[2];
	cnt[2] = 0;

	// (1, 1, 1, 1, 1) / (1, 1, 1, 1) / (1, 1, 1) / (1, 1) / (1)
	ile = (cnt[1] + 4) / 5;
	ans += ile;
	cnt[1] = 0;

	cout << ans << '\n';

	return 0;
}
