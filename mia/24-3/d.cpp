// Author: Olaf Surgut (surgutti)
// Created on 18-10-2024 14:35:52
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

const int N = 1000000 + 7;
const int mod = 1e9 + 7;

int fac[N];
int ifac[N];
int pw3[N];

int power(int a, int b) {
	int r = 1;
	for (; b; b >>= 1, (a *= a) %= mod)
		if (b & 1)
			(r *= a) %= mod;
	return r;
}

int C(int n, int k) {
	if (0 <= k && k <= n)
		return fac[n] * ifac[k] % mod * ifac[n - k] % mod;
	return 0;
}

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	pw3[0] = 1;
	FOR(i, 1, N - 1)
		pw3[i] = pw3[i - 1] * 3 % mod;

	fac[0] = 1;
	FOR(i, 1, N - 1)
		fac[i] = fac[i - 1] * i % mod;

	ifac[N - 1] = power(fac[N - 1], mod - 2);
	ROF(i, N - 2, 0)
		ifac[i] = ifac[i + 1] * (i + 1) % mod;

	int n, k;
	cin >> n >> k;
	
	if (k == 0) {
		cout << pw3[n] << '\n';
		return 0;
	}

	int ans = 0;
	FOR(f, 1, k) {
		if (n - 2 * f + 1 >= 0)
			(ans += C(k - 1, f - 1) * C(n - k - 1, f - 1) % mod * pw3[n - 2 * f + 1] % mod) %= mod;
		if (n - 2 * f + 1 >= 0)
			(ans += C(k - 1, f - 1) * C(n - k - 1, f - 2) % mod * pw3[n - 2 * f + 1] % mod) %= mod;
		if (n - 2 * f >= 0)
			(ans += C(k - 1, f - 1) * C(n - k - 1, f - 1) % mod * pw3[n - 2 * f] % mod) %= mod;
		if (n - 2 * f >= 0)
			(ans += C(k - 1, f - 1) * C(n - k - 1, f - 0) % mod * pw3[n - 2 * f] % mod) %= mod;
	}

	cout << ans << '\n';

	return 0;
}

