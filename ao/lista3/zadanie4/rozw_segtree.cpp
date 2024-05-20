#include <bits/stdc++.h>

using namespace std;

const int N = 500'000 + 7;

int n;
int tree[4 * N], tree_size;

void update(int p) {
	for (tree[p += tree_size] = 0; p >>= 1; ) {
		tree[p] = tree[2 * p] + tree[2 * p + 1];
	}
}

int next_on(int p, int l, int r, int u) {
	if (r < p || tree[u] == 0)
		return n + 1;
	
	if (l == r)
		return l;
	
	int m = (l + r) / 2;

	int L = next_on(p, l, m, 2 * u + 0);
	if (L <= n)
		return L;

	return next_on(p, m + 1, r, 2 * u + 1);
}

int main() {
	ios::sync_with_stdio(false), cin.tie(nullptr);

	cin >> n;

	tree_size = 1;
	while (tree_size <= n)
		tree_size <<= 1;

	for (int i = 1; i <= n; i++)
		tree[i + tree_size] = 1;
	
	for (int i = tree_size; i; i--)
		tree[i] = tree[2 * i] + tree[2 * i + 1];

	for (int i = 1; i <= n; i++) {
		int a; cin >> a;

		update(a);

		int nxt = next_on(a, 0, tree_size - 1, 1);
		if (nxt <= n)
			cout << nxt << '\n';
		else
			cout << "NIE\n";
	}
	
	return 0;
}
