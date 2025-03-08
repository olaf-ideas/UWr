#include <stdint.h>

#define u32 uint32_t

u32 bit_parity(u32 x) {
	x ^= x >> 16;
	x ^= x >> 8;
	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;
	return x & 1;
}

int main() {

	return 0;
}
