// Author: Olaf Surgut (surgutti)
// Created on 18-10-2024 14:17:31
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
		vector<int> u(n);
		for (int &x : u)
			cin >> x;

		map<int, int> fst, lst;
		FOR(i, 0, n - 1) {
			if (fst.count(u[i]) == 0)
				fst[u[i]] = i;
			lst[u[i]] = i;
		}

		while (k--) {
			int a, b;
			cin >> a >> b;
			
			if (lst.count(a) && lst.count(b) && fst[a] <= lst[b]) {
				cout << "YES\n";
			}
			else {
				cout << "NO\n";
			}
		}
	}

	return 0;
}

