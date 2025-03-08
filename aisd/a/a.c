// Olaf Surgut 345615
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define N 200000
#define M 10001

int n, i, l, m, r;
int* adj[M];
int nxt[N], mid[N], len[M], vis[M];
int ans_size, ans[3 * M];

int dfs(int16_t u) {
	int j, v;

	vis[u] = 1;
	for (j = len[u]; j--; ) {
		v = nxt[adj[u][j]];

		if (v == 0 || (!vis[v] && dfs(v))) {
			ans[3 * ans_size + 0] = v;
			ans[3 * ans_size + 1] = mid[adj[u][j]];
			ans[3 * ans_size + 2] = u;
			ans_size++;
			return 1;
		}
	}

	return 0;
}

int main() {
	scanf("%d", &n);

	for (i = 0; i < M; i++) {
		adj[i] = (int*) malloc(2 * sizeof *adj[i]);
	}

	for (i = 0; i < n; i++) {
		scanf("%d %d %d", &l, &m, &r);

		int k = len[l]++;
		if (k >= 2 && (k & (k - 1)) == 0) {
			adj[l] = (int*) realloc(adj[l], k * 2 * sizeof *adj[l]);
		}

		adj[l][k] = i;
		nxt[i] = r;
		mid[i] = m;
	}
	
	if (!dfs(0)) {
		puts("BRAK");
	}
	else {
		printf("%d\n", ans_size);
		for (i = 3 * ans_size; i--; ) {
			printf("%d ", ans[i]);
			if (i % 3 == 0)
				puts("");
		}
	}

	for (i = 0; i < M; i++)
		free(adj[i]);

	return 0;
}
