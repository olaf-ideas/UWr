#include <iostream>
#include <iomanip>
#include <openssl/sha.h>
#include <cstring>
#include "sparsepp/sparsepp/spp.h"

#define SHAPE1 "\
<html>\n\
<head>\n\
<style type=\"text/css\">\n\
   p.fgfgfgfg { font-style: italic; }\n\
</style>\n\
</head>\n\
<body>\n\
<p class=\"fgfgfgfg\">Jest wspaniale!</p>\n\
</body>\n\
</html>\n\
";

#define SHAPE2 "\
<html>\n\
<head>\n\
<style type=\"text/css\">\n\
   p.fgfgfgfg { font-style: italic; }\n\
</style>\n\
</head>\n\
<body>\n\
<p class=\"fgfgfgfg\">Jest fatalnie!</p>\n\
</body>\n\
</html>\n\
";

unsigned char hash[SHA256_DIGEST_LENGTH];
SHA256_CTX sha256;

uint64_t sha256_64(const char* data) {
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, data, strlen(data));
	SHA256_Final(hash, &sha256);

	/*
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		std::cout << std::hex << std::setw(2) << std::setfill('0') << (int) hash[i];
	}
	std::cout << '\n';
	// */

	uint64_t h = 0;
	for (int i = 0; i < 8; i++) {
		h = (h << 8) | hash[i];
	}

	return h >> (64 - 48);
}

char text1[] = SHAPE1;
char text2[] = SHAPE2;

spp::sparse_hash_map<uint64_t, int64_t> L, R;

// 32 bits -> instant
// 48 bits -> 3 minutes / 1.5GB

void apply_seed(char* text, uint64_t seed) {
	for (int j = 35 + 16 - 1; j >= 27 + 16; j--) {
		text[j] = text[j - 27 - 16 + 110] = char('a' + seed % 26);
		seed /= 26;
	}
}

int main(int argc, char** argv) {
	
	for (uint64_t seed = 0; ; seed++) {
		
		apply_seed(text1, seed);
		apply_seed(text2, seed);
		
		uint64_t h1 = sha256_64(text1);
		uint64_t h2 = sha256_64(text2);

		if (L.count(h2)) {
			apply_seed(text1, L[h2]);
			apply_seed(text2, seed);

			printf("1)\n%s\n", text1);
			printf("2)\n%s\n", text2);
			
			return 0;
		}

		if (R.count(h1)) {
			apply_seed(text1, seed);
			apply_seed(text2, R[h1]);

			printf("1)\n%s\n", text1);
			printf("2)\n%s\n", text2);
	
			printf("%u %u\n", L.size(), R.size());		
			return 0;
		}

		L[h2] = seed;
		R[h1] = seed;
	}

	return 0;
}
