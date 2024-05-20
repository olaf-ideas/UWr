#include <bits/stdc++.h>

using namespace std;

int main() {
	ios::sync_with_stdio(false), cin.tie(nullptr);

	int n;
	cin >> n;

	set<int> dysk;

	for (int i = 1; i <= n; i++)
		dysk.insert(i);
	
	for (int i = 1; i <= n; i++) {
		int a;
		cin >> a;

		auto it = dysk.lower_bound(a);

		if (next(it) != dysk.end())
			cout << *next(it) << '\n';
		else
			cout << "NIE\n";

		dysk.erase(it);
	}


	return 0;
}
