#include "testlib.h"

#include <iostream>

using namespace std;

const int MAX_C = 300000;

int n, a, b;

int dp[MAX_C], max_c;

int worse_scenario(int l, int r) {
	if (l == r)
		return 0;

	int len = r - l + 1, cost = max_c - 1;

	while (dp[cost - 1] >= len)
		cost--;
	
	return cost;
}

int main(int argc, char *argv[]) {
	registerInteraction(argc, argv);

	n = inf.readInt();
	a = inf.readInt();
	b = inf.readInt();
	
	cout << n << endl;
	cout << a << endl;
	cout << b << endl;
	
	for (int i = 0; i < max(a, b); i++)
		dp[i] = 1;
	
	max_c = max(a, b);
	
	while (dp[max_c - 1] < n) {
	    dp[max_c] = dp[max_c - a] + dp[max_c - b];
	    max_c++;
	}
	
	int l = 1, r = n;

	int cost = 0;
	for (int rep = 0; ; rep++) {
		ensuref(1 <= l && l <= r && r <= n, "Potencjalni kandydaci [%d, %d] nie tworzą poprawnego przedziału w [1, %d]", l, r, n);
		
		string c = ouf.readToken("N|W|!");
		
		if (c == "N") {
			int x = ouf.readInt(1, n);

			if (x <= l || r < x) {
				quitf(_wa, "Niepotrzebne zapytanie (? %d) poza przedziałem (%d, %d]", x, l, r); 
			}

			int before_cost = worse_scenario(l, r);

			int after_cost = max(a + worse_scenario(l, x - 1),
								b + worse_scenario(x, r));
		
			if (after_cost > before_cost) {
				quitf(_wa, "Po %d-tym zapytaniu pesymistyczny koszt wzrósł z %d na %d\n", rep, before_cost, after_cost);
			}

			if (rnd.next(0, 2) == 1) {
				cout << "tak" << endl;
				cost += a;
				r = x - 1;
			}
			else {
				cout << "nie" << endl;
				cost += b;
				l = x;
			}
		}
		else
		if (c == "W") {
			int x = ouf.readInt(1, n);

			if (x < l || r <= x) {
				quitf(_wa, "Niepotrzebne zapytanie (? %d) poza przedziałem [%d, %d)", x, l, r);
			}

			int before_cost = worse_scenario(l, r);

			int after_cost = max(a + worse_scenario(x + 1, r),
								b + worse_scenario(l, x));
			
			if (after_cost > before_cost) {
				quitf(_wa, "Po %d-tym zapytaniu pesymistyczny koszt wzrósł z %d na %d\n", rep, before_cost, after_cost);
			}

			if (rnd.next(0, 2) == 1) {
				cout << "tak" << endl;
				cost += a;
				l = x + 1;
			}
			else {
				cout << "nie" << endl;
				cost += b;
				r = x;
			}
		}
		else
		if (c == "!") {
			int x = ouf.readInt(1, n);
			
			if (l == x && x == r) {
			    ouf.readEoln();
				ouf.readEof();
				quitf(_ok, "OK, użyto: %d bajtalarów", cost);
			}
			else {
				quitf(_wa, "Błędna odpowiedź: %d, możliwe odpowiedzi leżą w przedziale: [%d %d]", x, l, r);
			}
		}
		else {
			quitf(_wa, "Unknown operation: '%c'", c);
		}
	}

	return 0;
}
