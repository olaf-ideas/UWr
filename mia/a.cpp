// Author: Olaf Surgut (surgutti)
// Created on 11-10-2024 14:47:19
#include <bits/stdc++.h>
using namespace std;

#ifdef DEBUG
auto&operator<<(auto&o,pair<auto,auto>p){return o<<"("<<p.first<<", "<<p.second<<")";}
auto operator<<(auto&o,auto x)->decltype(x.end(),o){o<<"{";int i=0;for(auto e:x)o<<","+!i++<<e;return o<<"}";}
#define debug(X...)cerr<<"["#X"]: ",[](auto...$){((cerr<<$<<"; "),...)<<endl;}(X)
#else
#define debug(...){}
#endif

#define endl '\n'
#define st first
#define nd second
#define pb push_back
#define sz(x) (int)(x).size()
#define all(x) (x).begin(), (x).end()
#define FOR(i, a, b) for (int i = (a); i <= (b); i++)
#define ROF(i, b, a) for (int i = (b); i >= (a); i--)
#define REP(i, n) for (int i = 0; i < (n); i++)

const int N = 100000 + 7;

int n, m, c[N];
vector<int> adj[N];

int vis[N];

int dfs(int u) {
	vis[u] = 1;
	int res = c[u];
	for (int v : adj[u])
		if (!vis[v])
			res = min(res, dfs(v));
	return res;
}

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m;
	FOR(i, 1, n)	cin >> c[i];
	REP(i, m) {
		int a, b;
		cin >> a >> b;
		adj[a].pb(b);
		adj[b].pb(a);
	}

	long long ans = 0;
	FOR(i, 1, n)
		if (!vis[i])
			ans += dfs(i);
	cout << ans << '\n';

	return 0;
}

