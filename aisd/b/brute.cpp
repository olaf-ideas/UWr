// Author: Olaf Surgut (surgutti)
// Created on 05-03-2025 01:14:11
#include "bits/stdc++.h"
using namespace std;

#define int long long
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

void solve() {
	int m, k;
	cin >> m >> k;

	set<int> S;
	FOR(i, 1, m) FOR(j, 1, m)
		S.insert(i * j);
	
	while (k--) {
		cout << *prev(S.end()) << '\n';
		S.erase(prev(S.end()));
	}
}

signed main() {
	cin.tie(0)->sync_with_stdio(0);

	int tt = 1;
	// cin >> tt;
	while (tt--)
		solve();

	return 0;
}

