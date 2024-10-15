// Author: Olaf Surgut (surgutti)
// Created on 11-10-2024 15:16:28
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

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int c, hr, hb, wr, wb;
	cin >> c >> hr >> hb >> wr >> wb;

	int ans = 0;
	for (int rep : {0, 1}) {
		for (int i = 0; i <= 1000000; i++) {
			// i * wr + j * wb <= c
			// j = (c - i * wr) / wb

			int j = max<int>(0, (c - i * wr) / wb);

			if (i * wr + j * wb <= c) {
				ans = max(ans, i * hr + j * hb);
			}
		}
		swap(hr, hb);
		swap(wr, wb);
	}

	cout << ans << '\n';
	
	return 0;
}

