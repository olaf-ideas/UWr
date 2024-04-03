#include "testlib.h"

#include <vector>
#include <map>

using namespace std;

/*
INPUT:
n m
s t
u_1 v_1 w_1
...
u_m v_m w_m

0 <= w_i <= 10**9

OUTPUT:
k 
s u_2 u_3 ... u_{k-1} t
*/

int main(int argc, char *argv[]) {
	setName("Najkrótsza ścieżka w grafie ważonym - Checker");
	registerTestlibCmd(argc, argv);

	int n = inf.readInt();
	int m = inf.readInt();
	int s = inf.readInt();
	int t = inf.readInt();

	map<pair<int, int>, int> edges;
	for (int i = 1; i <= m; i++) {
		int u = inf.readInt();
		int v = inf.readInt();
		int w = inf.readInt();

		if (edges.count(make_pair(u, v)) == 0 ||
			edges[make_pair(u, v)] > w)
			edges[make_pair(u, v)] = w;
	}
	
	long long ans_dist = 0;
	{
		int k = ans.readInt();
		ensure(2 <= k && k <= n);

		vector<int> path(k);

		for (int &u : path) {
			u = ans.readInt();

			if (u < 1 || n < u) {
				quitf(_fail, "Wierzchołek na ścieżce poza przedziałem [1, %d]", n);
			}
		}

		if (path[0] != s)
			quitf(_fail, "Ścieżka nie zaczyna się od %d", s);

		if (path.back() != t)
			quitf(_fail, "Ścieżka nie zaczyna się od %d", t);

		for (int i = 1; i < (int) path.size(); i++) {
			auto edge = make_pair(path[i - 1], path[i]);

			if (edges.count(edge) == 0) {
				quitf(_fail, "Krawędź %d %d nie istnieje", path[i - 1], path[i]);
			}

			ans_dist += edges[edge];
		}
	}

	vector<int> path(ouf.readInt(2, n));

	for (int &u : path)
		u = ouf.readInt(1, n);

	if (path[0] != s)
		quitf(_wa, "Ścieżka nie zaczyna się od %d", s);
	
	if (path.back() != t)
		quitf(_wa, "Ścieżka nie kończy się na %d", t);
	
	long long res_dist = 0;

	for (int i = 1; i < (int) path.size(); i++) {
		auto edge = make_pair(path[i - 1], path[i]);

		if (edges.count(edge) == 0) {
			quitf(_wa, "Krawędź %d %d nie istnieje", path[i - 1], path[i]);
		}

		res_dist += edges[edge];
	}

	if (res_dist < ans_dist) {
		quitf(_fail, "Odległość zawodnika (%lld) jest lepsza od rozwiązania (%lld)\n", res_dist, ans_dist);
	}

	if (res_dist > ans_dist) {
		quitf(_wa, "Błędna odległość %lld, zamiast %lld\n", res_dist, ans_dist);
	}

	quitf(_ok, "Odpowiedź = %lld\n", ans_dist);
}

