// Author: Olaf Surgut (surgutti)
// Created on 11-10-2024 14:55:55
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

char xd(char a, char b) {
	char c = 'a';
	while (c == a || c == b)
		c++;
	return c;
}

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, t;
	cin >> n >> t;

	string a, b;
	cin >> a >> b;

	vector<int> diff, same;
	
	REP(i, n) {
		if (a[i] == b[i]) {
			same.push_back(i);
		}
		else {
			diff.push_back(i);
		}
	}

	string C(n, '?');

	if (sz(diff) % 2 == 1) {
		if (t > 0) {
			int i = diff.back();
			char cc = xd(a[i], b[i]);
			
			C[i] = cc;
			t--;
			diff.pop_back();
		}
		else {
			cout << "-1\n";
			return 0;
		}
	}

	if (sz(diff) <= t) {
		debug("xd");
		for (int i : diff) {
			char cc = xd(a[i], b[i]);
			C[i] = cc;
			t--;
		}

		debug(C);

		for (int i : same) {
			if (t > 0) {
				char cc = xd(a[i], b[i]);
				C[i] = cc;
				t--;
			}
			else {
				C[i] = a[i];
			}
		}

		for (char c : C)
			assert(c != '?');
		cout << C << '\n';
		return 0;
	}

	assert(sz(diff) % 2 == 0);
	if (t < sz(diff) / 2) {
		cout << "-1\n";
		return 0;
	}

	for (int i = 0; i < sz(diff); i++) {
		if (i % 2 == 0) {
			C[diff[i]] = a[diff[i]];
		}
		else {
			C[diff[i]] = b[diff[i]];
		}
	}

	debug("here", t, sz(diff) / 2);
	t -= sz(diff) / 2;

	while (t > 0 && sz(diff) > 0) {
		int i = diff.back();
		diff.pop_back();
		int j = diff.back();
		diff.pop_back();

		C[i] = xd(a[i], b[i]);
		C[j] = xd(a[j], b[j]);
	
		t--;
	}

	while (t > 0 && sz(same) > 0) {
		int i = same.back();
		same.pop_back();

		C[i] = xd(a[i], b[i]);
		t--;
	}

	while (sz(same) > 0) {
		int i = same.back();
		same.pop_back();

		C[i] = a[i];
	}

	if (t != 0) {
		cout << "-1\n";
		return 0;
	}

	cout << C << '\n';

	return 0;
}

