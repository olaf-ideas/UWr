// Author: Olaf Surgut (surgutti)
// Created on 11-10-2024 14:50:32
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

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	#define int long long

	int n, a, b, k;
	cin >> n >> a >> b >> k;
	
	int ans = 0;
	vector<int> heh;
	FOR(i, 1, n) {
		int h;
		cin >> h;

		int left = h - (h - 1) / (a + b) * (a + b);
			
		if (left <= a) {
			ans++;
		}
		else {
			heh.push_back((left - 1) / a);
		}
	}

	sort(heh.begin(), heh.end());
	for (int x : heh) {
		if (x <= k) {
			k -= x;
			ans++;
		}
	}

	cout << ans << '\n';

	return 0;
}

