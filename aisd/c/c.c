#include <stdio.h>

#define P 100

int n, p, m;
char line[4];
int bad[P];

int good_cnt;
int good[1 << 15];
int dp[2][1 << 10];

int main() {
	scanf("%d%d%d", &n, &p, &m);

	for (int i = 0; i < p; i++) {
		for (int j = 0; j < 3; j++) {
			scanf("%s", line);
			for (int k = 0; k < 3; k++) {
				if (line[k] == 'x') {
					bad[i] |= 1 << (j + 5 * k);
				}
			}
		}
	}

	for (unsigned i = 0; i < (1 << 15); i++) {
		int ok = 1;

		const int full = 0b001110011100111;
		for (int j = 0; j < p && ok; j++) {
			for (int k = 0; k < 3; k++) {
				if ((( i >> k) &  bad[j]) == bad[j] &&
					(((~i >> k) & ~bad[j]) & full) == (~bad[j] & full)) {
					ok = 0;
					break;
				}
			}
		}

		if (ok) {
			good[good_cnt++] = i;
		}
	}

	fprintf(stderr, "%d\n", good_cnt);

	for (int i = 0; i < (1 << 10); i++) {
		dp[1][i] = 1;
	}

	for (int i = 2; i < n; i++) {
		int* prv = dp[(i ^ 1) & 1];
		int* nxt = dp[i & 1];

		for (int j = 0; j < (1 << 10); j++) {
			nxt[j] = 0;
		}

		for (int j = 0; j < good_cnt; j++) {
			int k = good[j];
			nxt[k >> 5] += prv[k & ((1 << 10) - 1)];
			if (nxt[k >> 5] >= m) {
				nxt[k >> 5] -= m;
			}
		}
	}

	long long ans = 0;
	for (int i = 0; i < (1 << 10); i++) {
		ans += dp[(n - 1) & 1][i];
	}

	printf("%lld\n", ans % m);

	return 0;
}
