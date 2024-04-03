#include <bits/stdc++.h>

using namespace std;

#include "unistd.h"

const int MAX_N = 100'000;
const int MAX_M = 200'000;

mt19937 rng(getpid());

int randint(int a, int b) {
	return uniform_int_distribution<int>(a, b)(rng);
}

int main() {
	ios::sync_with_stdio(false), cin.tie(nullptr);

	int n = randint(1, MAX_N);
	int k = randint(2, n);
	int m = randint(k - 1, MAX_M);

	vector<int> path(n);
	iota(path.begin(), path.end(), 1);
	shuffle(path.begin(), path.end(), rng);

	cout << n << ' ' << m << '\n';
	cout << path[0] << ' ' << path[k] << '\n';

	vector<tuple<int, int, int>> edges;
	for (int i = 0; i + 1 <= k; i++) {
		int u = path[i];
		int v = path[i + 1];

		edges.emplace_back(u, v, randint(1, 100));
	}

	while ((int) edges.size() < m) {
		int u = randint(1, n - 1);
		int v = randint(u + 1, n);

		if (rng() & 1)
			swap(u, v);

		edges.emplace_back(u, v, 1000);
	}

	shuffle(edges.begin(), edges.end(), rng);

	for (auto [u, v, w] : edges) {
		cout << u << ' ' << v << ' ' << w << '\n';
	}
	
	return 0;
}
