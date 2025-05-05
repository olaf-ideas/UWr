#include <stdlib.h>

rax = rdi + rsi
rdi ^= rax
rsi ^= rax
rax = rdi
rax &= rsi
rax >>= 63

long decode(long long x, long long y) {
	long long z = x + y;
	x ^= z;
	y ^= y;
	z = x & y;
	return z >> 63;
}

int main() {

	return 0;
}
