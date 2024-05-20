#include <bits/stdc++.h>

using namespace std;

const int N = 100'000;

const long long inf = 1e18L;

int n, m;
int s, t;
vector<pair<int, int>> adj[N + 1];

long long dist[N + 1];
int prv[N + 1];

int main() {
	ios::sync_with_stdio(false), cin.tie(nullptr);

	cin >> n >> m;
	cin >> s >> t;
	for (int i = 1; i <= m; i++) {
		int a, b, w;
		cin >> a >> b >> w;
		adj[b].emplace_back(a, w);
	}

	for (int i = 1; i <= n; i++)
		dist[i] = inf;

	set<pair<long long, int>> S;
	S.emplace(dist[t] = 0, t);

	while (S.size()) {
		auto [d, u] = *S.begin();
		S.erase(S.begin());

		for (auto [v, w] : adj[u]) {
			if (dist[v] > dist[u] + 1) {
				dist[v] = dist[u] + 1;
				prv[v] = u;
				S.emplace(dist[v], v);
			}
		}
	}

	vector<int> path;
	int u = s;
	while (u != t) {
		path.push_back(u);
		u = prv[u];
	}
	path.push_back(u);

	cout << path.size() << '\n';
	for (int v : path)
		cout << v << ' ';
	cout << '\n';

}
