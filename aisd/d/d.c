#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define N 1000001
#define X 10000000LL
#define P 10000019

uint64_t xorshift64_state = 0x2137;
uint64_t xorshift64() {
	uint64_t x = xorshift64_state;
	x ^= x << 7;
	x ^= x << 9;
	return xorshift64_state = x;
}

int n, x[N], y[N];

long long dist2(int i, int j) {
	long long dx = x[i] - x[j];
	long long dy = y[i] - y[j];

	return dx * dx + dy * dy;
}

int next[N], head[P];

long long ans_d;
int ans_i = 1;
int ans_j = 2;

void update_ans(int i, int j) {
	long long d = dist2(i, j);
	if (ans_d > d) {
		ans_d = d;
		ans_i = i;
		ans_j = j;
	}
}

int main() {
	
	scanf("%d", &n);
	for (int i = 1; i <= n; i++)
		scanf("%d %d", x + i, y + i);
	
	ans_d = dist2(1, 2);
	for (int i = 1; i <= n; i++) {
		int a = xorshift64() % n + 1;
		int b = xorshift64() % n + 1;

		if (a != b) {
			update_ans(a, b);
		}

		int j = xorshift64() % n + 1;
		if (i != j) {
			update_ans(i, j);
		}
	}

	long long rng_d = (long long) ceil(sqrt(ans_d));
	for (int i = 1; i <= n && ans_d > 0; i++) {
		int bucket_x = X + x[i] / rng_d;
		int bucket_y = X + y[i] / rng_d;
		
		for (int dx = -1; dx <= +1; dx++) {
			for (int dy = -1; dy <= +1; dy++) {
				int neigh_x = bucket_x + dx;
				int neigh_y = bucket_y + dy;

				xorshift64_state = neigh_x * 2 * X + neigh_y;
				int neigh_hash = xorshift64() % P;

				for (int j = head[neigh_hash]; j; j = next[j]) {
					update_ans(i, j);
				}
			}
		}
	
		xorshift64_state = bucket_x * 2 * X + bucket_y;
		int bucket_hash = xorshift64() % P;

		next[i] = head[bucket_hash];
		head[bucket_hash] = i;
	}
	
	printf("%d %d\n%d %d\n", x[ans_i], y[ans_i], x[ans_j], y[ans_j]);

	return 0;
}
