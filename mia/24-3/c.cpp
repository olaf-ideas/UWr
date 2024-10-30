// Author: Olaf Surgut (surgutti)
// Created on 18-10-2024 14:20:11
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
#define eb emplace_back
#define sz(x) (int)(x).size()
#define all(x) (x).begin(), (x).end()
#define FOR(i, a, b) for (int i = (a); i <= (b); i++)
#define ROF(i, b, a) for (int i = (b); i >= (a); i--)
#define REP(i, n) for (int i = 0; i < (n); i++)
#define int int64_t

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int tt = 1;
	cin >> tt;
	while (tt--) {
		int n, k;
		cin >> n >> k;
		vector<int> a(n);
		for (int& x : a)
			cin >> x;

		int ans = 1e18L;
		for (int mn = 0; mn <= 3000; mn++) {
			int mx = 0;
			FOR(i, 0, n - 1) {
				if (mn == 0 && a[i] >= k) {
					mx = 1e18L;
					break;
				}

				int p = 0;
				if (mn == 0) {
					p = a[i] + 1;
				}
				else {
					p = min(k, max<int>(1, a[i] / mn));
				}

				debug(mn, i, a[i] / p, p);
				while (p >= 1 && (a[i] / (p + 1)) < mn)
					p--;
				while (p + 1 <= k && (a[i] / (p + 1)) >= mn)
					p++;
				
				if (p < 1 || p > k) {
					mx = 1e18L;
					break;
				}

				mx = max(mx, a[i] / p);
			}
			ans = min(ans, mx - mn);
		}

		cout << ans << '\n';
	}

	return 0;
}

