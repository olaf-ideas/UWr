#include <bits/stdc++.h>

using namespace std;

const int N = 500'000 + 7;

int n, repr[N];

int get_repr(int u) {
	if (u == repr[u])
		return u;
	return repr[u] = get_repr(repr[u]);
}

int main() {
	ios::sync_with_stdio(false), cin.tie(nullptr);

	cin >> n;

	for (int i = 1; i <= n; i++)
		repr[i] = i;

	for (int i = 1; i <= n; i++) {
		int a;
		cin >> a;

		int nxt = get_repr(a) + 1;

		if (nxt <= n)
			cout << nxt << '\n';
		else
			cout << "NIE\n";

		repr[a - 1] = get_repr(a);
	}

	return 0;
}

