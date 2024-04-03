#include "testlib.h"

#include <set>

using namespace std;

const int MAX_N = 500'000;

int uf[MAX_N + 1];

int get(int u) {
	return u == uf[u] ? u : uf[u] = get(uf[u]);
}

int main(int argc, char *argv[]) {
	registerValidation(argc, argv);

	int n = inf.readInt(1, MAX_N, "n");
	inf.readEoln();

	set<pair<int, int>> edges;
	vector<pair<vector<int>, int>> phones;

	int p = 0;
	for (int i = 1; i <= n; i++) {
		int k_i = inf.readInt(1, n, "k_i");
		
		vector<int> vv;
		
		int last = 0;
		for (int j = 1; j <= k_i; j++) {
			inf.readSpace();
			int v = inf.readInt(last + 1, n, "{v_i}_j");

			ensuref(v != i, "W liście zapisanych numerów %d-tego mieszkańca występuje %d", i, i);
			edges.emplace(i, v);
			vv.push_back(v);
			
			last = v;
		}

		vv.push_back(i);
		sort(vv.begin(), vv.end());

		phones.emplace_back(vv, i);

		p += k_i;
		inf.readEoln();
	}

	for (auto [a, b] : edges) {
		ensuref(edges.count(make_pair(b, a)) > 0, "Istnieje połączenie <%d, %d>, a <%d, %d> nie istnieje", a, b, b, a);
	}

	sort(phones.begin(), phones.end());
	
	for (int i = 1; i <= n; i++)
		uf[i] = i;

	set<pair<int, int>> clique_edges;
	set<int> cliques;

	for (int i = 0; i + 1 < (int) phones.size(); i++) {
		auto [A, a] = phones[i];
		auto [B, b] = phones[i + 1];

		if (A == B) {
			uf[get(a)] = get(b);
		}
	}
	
	for (auto [a, b] : edges) {
		if (get(a) != get(b)) {
			int u = get(a);
			int v = get(b);

			if (u > v)
				swap(u, v);

			clique_edges.emplace(u, v);
		}
	}
	
	for (int i = 1; i <= n; i++)
		cliques.insert(get(i));

	for (int i = 1; i <= n; i++)
		uf[i] = i;

	for (auto [a, b] : clique_edges) {
		a = get(a);
		b = get(b);

		ensuref(a != b, "Graf nie jest drzewem");

		uf[a] = b;
	}

	for (int u : cliques) {
		ensuref(get(u) == get(*cliques.begin()), "Graf nie jest spójny");
	}

	ensuref(0 <= p, "p < 0");
	ensuref(p <= 1'000'000, "p > 1'000'000");

	inf.readEof();

	return 0;
}
