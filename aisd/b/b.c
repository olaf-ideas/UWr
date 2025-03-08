// Olaf Surgut 345615
#include <stdio.h>

#define M 1000001

int m, y[M], h[M];

int cmp(int i, int j) {
	return (long long) h[i] * y[h[i]] < (long long) h[j] * y[h[j]];
}

void update(int i) {
	int j, k = i;
	do {
		j = k;
		if ((j << 1 | 0) <= m && cmp(k, j << 1 | 0))
			k = j << 1 | 0;
		if ((j << 1 | 1) <= m && cmp(k, j << 1 | 1))
			k = j << 1 | 1;
	
		int tmp = h[j];
		h[j] = h[k];
		h[k] = tmp;
	} while (j != k);
}

int main() {
	
	int i, k;
	scanf("%d %d", &m, &k);

	for (i = m; i >= 1; i--) {
		h[i] = i;
		y[i] = m;

		update(i);
	}

	long long prv = -1, now;
	while (k) {
		now = (long long) h[1] * y[h[1]];
		
		if (now != prv) {
			printf("%lld\n", now);
			k--;
		}

		y[h[1]]--;
		update(1);

		prv = now;
	}

	return 0;
}
