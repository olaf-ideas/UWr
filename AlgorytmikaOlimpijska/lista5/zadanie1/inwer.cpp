#include "testlib.h"

#include <set>

using namespace std;

int main(int argc, char *argv[]) {
	registerValidation(argc, argv);

	int n = inf.readInt(1, 500'000, "n");
	inf.readEoln();

	set<pair<int, int>> edges;

	int p = 0;
	for (int i = 1; i <= n; i++) {
		int k_i = inf.readInt(1, n, "k_i");
		
		int last = 0;
		for (int j = 1; j <= k_i; j++) {
			inf.readSpace();
			int v = inf.readInt(last + 1, n, "{v_i}_j");

			ensuref(v != i, "W liście zapisanych numerów i-tego mieszkańca występuje i");
			edges.emplace(i, v);

			last = v;
		}

		p += k_i;
		inf.readEoln();
	}

	for (auto [a, b] : edges) {
		ensuref(edges.count(make_pair(b, a)) > 0, "Istnieje połączenie <a, b>, ale <b, a> nie");
	}

	ensuref(0 <= p, "p < 0");
	ensuref(p <= 1'000'000, "p > 1'000'000");

	inf.readEof();

	return 0;
}
