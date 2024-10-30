#include <iostream>
#include <iomanip>
#include <openssl/sha.h>
#include <cstring>

char text1[] = "\
<html>\n\
<head>\n\
<style type=\"text/css\">\n\
   p { color: #02d5df; }\n\
</style>\n\
</head>\n\
<body>\n\
<p>Jest wspaniale!</p>\n\
</body>\n\
</html>\n\
";

char text2[] = "\
<html>\n\
<head>\n\
<style type=\"text/css\">\n\
   p { color: #0b910a; }\n\
</style>\n\
</head>\n\
<body>\n\
<p>Jest wspaniale!</p>\n\
</body>\n\
</html>\n\
";

#define SHAPE "\
<html>\n\
<head>\n\
<style type=\"text/css\">\n\
   b { color: #000000; }\n\
   p { color: #000000; }\n\
   i { color: #000000; }\n\
</style>\n\
</head>\n\
<body>\n\
<p>A</p>\n\
<b>B</b>\n\
<i>C</i>\n\
</body>\n\
</html>\n\
";

unsigned char hash[SHA256_DIGEST_LENGTH];
SHA256_CTX sha256;

uint64_t sha256_64(const char* data) {
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, data, strlen(data));
	SHA256_Final(hash, &sha256);

	uint64_t h = 0;
	for (int i = 0; i < 8; i++) {
		h = (h << 8) | hash[i];
	}

	return h;
}

void gen_html(uint64_t h, char* X) {
	// scrable for better uniform distribution
	h = (h ^ (h >> 30)) * 0xbf58476d1ce4e5b9;
    h = (h ^ (h >> 27)) * 0x94d049bb133111eb;
    h = h ^ (h >> 31);

	static constexpr char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

	X[53] = hex[h & 15]; h >>= 4;
	X[54] = hex[h & 15]; h >>= 4;
	X[55] = hex[h & 15]; h >>= 4;
	X[56] = hex[h & 15]; h >>= 4;
	X[57] = hex[h & 15]; h >>= 4;
	X[58] = hex[h & 15]; h >>= 4;
	
	X[78] = hex[h & 15]; h >>= 4;
	X[79] = hex[h & 15]; h >>= 4;
	X[80] = hex[h & 15]; h >>= 4;
	X[81] = hex[h & 15]; h >>= 4;
	X[82] = hex[h & 15]; h >>= 4;
	X[83] = hex[h & 15]; h >>= 4;

	X[103] = hex[h & 15]; h >>= 4;
	X[104] = hex[h & 15]; h >>= 4;
	X[105] = hex[h & 15]; h >>= 4;
	X[106] = hex[h & 15]; h >>= 4;
	X[107] = hex[h & 15]; h >>= 4;
	X[108] = hex[h & 15]; h >>= 4;
}

char X[] = SHAPE;
uint64_t hX;

char Y[] = SHAPE;
uint64_t hY;

int main(int argc, char** argv) {

	uint64_t seed = atoll(argv[1]);	

	printf("Starting with seed = %lu\n", seed);

	// initial guess
	gen_html(seed, X);
	hX = sha256_64(X);

	std::cerr << X[52] << ' ' << X[53] << ' ' << X[54] << ' ' << X[55] << '\n';

	gen_html(seed, Y);
	hY = sha256_64(Y);
	
	while (true) {
		gen_html(hX, X);
		hX = sha256_64(X);
		gen_html(hX, X);
		hX = sha256_64(X);

		gen_html(hY, Y);
		hY = sha256_64(Y);

		if (hX == hY) {
			break;
		}
	}

	printf("Found collision\n");

	gen_html(seed, X);
	hX = sha256_64(X);

	while (hX != hY) {
		gen_html(hX, X);
		hX = sha256_64(X);
		gen_html(hY, Y);
		hY = sha256_64(Y);
	}
	
	printf("h(X) = %lu\n", sha256_64(X));
	printf("h(Y) = %lu\n", sha256_64(Y));

	printf("%s\n", X);
	printf("-----\n");
	printf("%s\n", Y);

	return 0;
}
