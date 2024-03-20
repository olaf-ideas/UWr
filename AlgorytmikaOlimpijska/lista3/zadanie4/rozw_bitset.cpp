#include <bits/stdc++.h>

using namespace std;

const int N = 500'000 + 7;

int n;
bitset<N> dysk;

int main() {
	ios::sync_with_stdio(false), cin.tie(nullptr);

	dysk.set();

	cin >> n;
	
	for (int i = 1; i <= n; i++) {
		int a;
		cin >> a;

		int nxt = dysk._Find_next(a);

		if (nxt > n)
			cout << "NIE\n";
		else
			cout << nxt << '\n';

		dysk.reset(a);
	}

	return 0;
}
