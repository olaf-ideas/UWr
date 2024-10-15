// Author: Olaf Surgut (surgutti)
// Created on 11-10-2024 15:36:26
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
#define pb push_bask
#define sz(x) (int)(x).size()
#define all(x) (x).begin(), (x).end()
#define FOR(i, a, b) for (int i = (a); i <= (b); i++)
#define ROF(i, b, a) for (int i = (b); i >= (a); i--)
#define REP(i, n) for (int i = 0; i < (n); i++)

#define int long long

const int T = 1 << 18;

pair<int, int> tree[T << 1];

const int inf = 1e9;

void update(int p, pair<int, int> v) {
	for (tree[p += T] = v; p >>= 1; )
		tree[p] = min(tree[p << 1 | 0], tree[p << 1 | 1]);
}

pair<int, int> query(int l, int r) {
	pair<int, int> res{inf, inf};
	for (l += T, r += T + 1; l < r; l >>= 1, r >>= 1) {
		if (l & 1)
			res = min(res, tree[l++]);
		if (r & 1)
			res = min(res, tree[--r]);
	}
	return res;
}

int c[T], w[T];
int A[T], B[T];

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;

	FOR(i, 1, n)
		cin >> c[i];
	
	FOR(i, 1, n)
		cin >> w[i];

	FOR(i, 1, n)
		update(i, make_pair(inf, inf));

	int ans = 0;
	FOR(i, 1, n) {
		
		A[i] = c[i] / 100;
		B[i] = c[i] % 100;

		c[i] -= A[i] * 100;
		
		if (c[i] == 0)
			continue;
		
		update(i, make_pair(w[i] * (100 - B[i]), i));

		while (m < c[i]) {
			auto [x, j] = query(1, i);
			ans += x;
			update(j, make_pair(inf, inf));
			A[j] += 1;
			B[j] = 0;
			m += 100;
		}

		m -= c[i];
	}

	cout << ans << '\n';
	FOR(i, 1, n) {
		cout << A[i] << ' ' << B[i] << '\n';
	}

	return 0;
}

