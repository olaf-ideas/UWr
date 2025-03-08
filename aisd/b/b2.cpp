// Author: Olaf Surgut (surgutti)
// Created on 06-03-2025 20:15:21
#include "bits/stdc++.h"
using namespace std;

#define ll long long
#define ld long double

#define endl '\n'
#define st first
#define nd second
#define pb push_back
#define eb emplace_back
#define sz(x) (int)(x).size()
#define all(x) begin(x),end(x)
#define FOR(i,l,r) for(int i=(l);i<=(r);i++)
#define ROF(i,r,l) for(int i=(r);i>=(l);i--)

auto& operator<<(auto &o, pair<auto, auto> p) {
	return o << "(" << p.st << ", " << p.nd << ")";}
auto operator<<(auto &o, auto x)->decltype(end(x), o) {
	o << "{"; int i=0; for (auto e : x) o << ","+!i++ << e;
	return o << "}";}

#ifdef LOCAL
#define debug(x...) cerr << "[" #x "]: ", [](auto...$) { \
	((cerr << $ << "; "),...) << endl; }(x)
#else
#define debug(...)
#endif

#define rep(i,a,b) for(int i = a; i < (b); i++)
using pii = pair<int, int>;
using vi = vector<int>;

signed main() {
	cin.tie(0)->sync_with_stdio(0);

	int m, k;
	cin >> m >> k;

	set<tuple<ll, int, int>> S;
	FOR(i, 1, m)
		S.emplace((ll) i * m, i, m);
	
	ll last = -1;
	while (k > 0) {
		auto [x, i, j] = *prev(S.end());
		S.erase(prev(S.end()));
	
		if (last != x) {
			cout << x << '\n';
			last = x;
			k--;
		}

		j--;
		S.emplace((ll) i * j, i, j);
	}

	return 0;
}

