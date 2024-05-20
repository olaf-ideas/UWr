#include <bits/stdc++.h>

using namespace std;

int cnt[5], ile;

int main() {
	ios::sync_with_stdio(false), cin.tie(nullptr);

	for (int i = 1; i <= 4; i++)
		cin >> cnt[i];

	int ans = 0;

	// (4)
	ans += cnt[4];
	cnt[4] = 0;

	// (3, 1) / (3)
	cnt[1] = max(0, cnt[1] - cnt[3]);
	ans += cnt[3];
	cnt[3] = 0;

	// (2, 2)
	ans += cnt[2] / 2;
	cnt[2] -= (cnt[2] / 2) * 2;

	// (2, 1, 1)
	ile = min(cnt[2], cnt[1] / 2);
	ans += ile;
	cnt[2] -= ile;
	cnt[1] -= ile * 2;

	// (2, 1) / (2)
	cnt[1] = max(0, cnt[1] - cnt[2]);
	ans += cnt[2];
	cnt[2] = 0;
	
	// (1, 1, 1, 1) / (1, 1, 1) / (1, 1) / (1)
	ans += (cnt[1] + 3) / 4;
	cnt[1] = 0;

	cout << ans << '\n';
}
