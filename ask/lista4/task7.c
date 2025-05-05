
#define u32 unsigned

u32 to_bin(u32 x) {
	x =  (x & 0x0F0F0F0F0F0F0F0F) +
		((x & 0xF0F0F0F0F0F0F0F0) << 3) +
		((x & 0xF0F0F0F0F0F0F0F0) << 1);

	x =  (x & 0x00FF00FF00FF00FF) +
		((x & 0xFF00FF00FF00FF00) << 3) +
		((x & 0xFF00FF00FF00FF00) << 1);
	
	x =  (x & 0x0000FFFF0000FFFF) +
		((x & 0xFFFF0000FFFF0000) << 3) +
		((x & 0xFFFF0000FFFF0000) << 1);
	
	x =  (x & 0x00000000FFFFFFFF) +
		((x & 0xFFFFFFFF00000000) << 3) +
		((x & 0xFFFFFFFF00000000) << 1);
	
	return x;
}

u32 to_bcd(u32 x) {

}

u32 bcd_add(u32 x, u32 y) {
	return to_bcd(to_bin(x) + to_bin(y));
}

int main() {

}
