// Author: Olaf Surgut (surgutti)
// Created on 11-11-2024 21:55:59
#include <bits/stdc++.h>
using namespace std;

#ifdef DEBUG
auto&operator<<(auto&o,pair<auto,auto>p){return o<<"("<<p.first<<", "<<p.second<<")";}
auto operator<<(auto&o,auto x)->decltype(x.end(),o){o<<"{";int i=0;for(auto e:x)o<<","+!i++<<e;return o<<"}";}
#define debug(X...)cerr<<"["#X"]: ",[](auto...$){((cerr<<$<<"; "),...)<<endl;}(X)
#else
#define debug(...){}
#endif

#define int long long
#define ll long long
#define ld long double

#define endl '\n'
#define st first
#define nd second
#define pb push_back
#define eb emplace_back
#define sz(x) (int)(x).size()
#define all(x) (x).begin(), (x).end()
#define FOR(i, a, b) for (int i = (a); i <= (b); i++)
#define ROF(i, b, a) for (int i = (b); i >= (a); i--)

#define rep(i, a, b) for (int i = (a); i < (b); i++)
typedef vector<int> vi;
typedef pair<int, int> pii;

vector<ld> ch {
	0.763907023,
	0.543852762,
	0.196247370,
	0.009220859
};

vector<ld> a {
	0.605426053,
	0.055322784,
	0.004819076,
	0.000399783
};

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cout << fixed << setprecision(9);

	for (int p = 1; p <= 5; p++) {
		cout << "p: " << p << '\n';
		for (int i = 0; i + 1 < sz(a); i++) {
			cout << a[i + 1] / pow(a[i], p) << ' ';
		}
		cout << '\n';
		for (int i = 0; i + 1 < sz(ch); i++) {
			cout << ch[i + 1] / pow(ch[i], p) << ' ';
		}
		cout << '\n';
	}

	ld A = (0.091378268 + 0.087108342 + 0.082958434) / 3;
	

	return 0;
}

