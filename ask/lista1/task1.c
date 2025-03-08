#include <stdint.h>

#define u32 uint32_t

u32 copy_bit(u32 x, u32 i, u32 k) {
	return (x & ~(1U << k)) | ((x >> i & 1) << k);
}

int main() {
	
	return 0;
}
