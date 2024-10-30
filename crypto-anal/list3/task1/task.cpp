#include <iostream>
#include <iomanip>
#include <openssl/sha.h>
#include <cstring>

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

	return h >> (64 - 32);
}

char original[] = SHAPE1;
char text[] = SHAPE2;

int main(int argc, char** argv) {
	
	uint64_t h_target = sha256_64(original);

	/*
	for (int i = 0; i < strlen(text); i++) {
		if (text[i] == 'f') {
			std::cerr << "!" << i << '\n';
		}
	}
	*/

	for (uint64_t seed = 0; ; seed++) {
		uint64_t now = seed;
		for (int j = 35 + 16 - 1; j >= 27 + 16; j--) {
			text[j] = text[j - 27 - 16 + 110] = char('a' + now % 26);
			now /= 26;
		}

		if (h_target == sha256_64(text)) {
			printf("%s\n", text);
			return 0;
		}
	}

	return 0;
}
