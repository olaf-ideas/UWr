// Author: Olaf Surgut (surgutti)
// Created on 03-04-2025 15:48:32
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

signed main() {
	cin.tie(0)->sync_with_stdio(0);

	int n;
	cin >> n;
	vector<pii> pts(n);
	for (auto& [x, y] : pts)
		cin >> x >> y;
	
	int ans = 1e18;

	rep(i, 0, n) {
		auto [x1, y1] = pts[i];
		rep(j, i + 1, n) {
			auto [x2, y2] = pts[j];

			int dx = x1 - x2;
			int dy = y1 - y2;

			ans = min(ans, dx * dx + dy * dy);
		}
	}

	cout << ans << '\n';

	return 0;
}

