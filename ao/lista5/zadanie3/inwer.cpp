#include "testlib.h"

const int MAX_N = 50'000;
const int MAX_M = 100'000;

vector<int> adj[MAX_N + 1];

int vis[MAX_N + 1];
int pre[MAX_N + 1];
int low[MAX_N + 1];
int tim = 1;

vector<int> sta;

int scc_id[MAX_N + 1];
int scc_sz[MAX_N + 1];
int scc_cnt;

vector<int> scc_adj[MAX_N + 1];

void dfs(int u) {
	vis[u] = 1;
	pre[u] = low[u] = tim++;
	sta.push_back(u);

	for (int v : adj[u]) {
		if (vis[v] == 0) {
			dfs(v);
			low[u] = min(low[u], low[v]);
		}
		else
		if (vis[v] == 1) {
			low[u] = min(low[u], pre[v]);
		}
	}

	if (low[u] == pre[u]) {
		int v = -1, sz = 0;
		while (u != v) {
			v = sta.back();
			sta.pop_back();
			scc_id[v] = scc_cnt;
			vis[v] = 2;
			sz++;
		}

		scc_sz[++scc_cnt] = sz;
	}
}

int main(int argc, char *argv[]) {
	registerValidation(argc, argv);

	int n = inf.readInt(2, MAX_N, "n");
	inf.readSpace();
	int m = inf.readInt(1, MAX_M, "m");
	inf.readEoln();

	vector<pair<int, int>> edges;
	for (int i = 1; i <= m; i++) {
		int a = inf.readInt(1, n, "a_i");
		inf.readSpace();
		int b = inf.readInt(1, n, "b_i");
		inf.readEoln();

		auto edge = make_pair(a, b);

		ensuref(a != b, "a_%d = b_%d", i, i);
		ensuref(edges.find(edge) == edges.end(), "Krawędź <%d,%d> już pojawiła się na wejściu", a b);
		
		edges.emplace(a, b);
		adj[a].push_back(b);
	}

	inf.readEof();

	for (int i = 1; i <= n; i++) {
		if (vis[i] == 0) {
			dfs(i);
		}
	}

	for (int i = 1; i <= n; i++) {
		for (int v : adj[i]) {
			if (scc_id[i] != scc_id[v]) {
				scc_adj[scc_id[i]].push_back(scc_id[v]);
			}
		}
	}

	for (int i = 1; i <= scc_cnt; i++) {
		scc_adj[i].erase(unique(scc_adj[i].begin(), scc_adj[i].end()), scc_adj[i].end());

		for (int v : scc_adj[i]) {
			scc_sz[i] += scc_sz[v];
		}
	}

	tim = 4;
	for (int i = 1; i <= n; i++) {
		int ans = scc_sz[scc_id[i]] - 1;

		int brute = 0;

		vector<int> que{i};
		
		for (int j = 0; j < (int) que.size(); j++) {
			int u = que[j];

			for (int v : adj[u]) {
				if (vis[v] != tim) {
					vis[v] = tim;
				}
			}
		}

		ensuref(ans == brute, "%d-ty wierzchołek nie jest sprawiedliwy", i);
	}

	return 0;
}
