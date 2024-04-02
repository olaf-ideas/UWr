#include "testlib.h"

int main(int argc, char *argv[]) {
	registerTestlibCmd(argc, argv);

	int n = inf.readInt(1, 1000 * 1000 * 1000, "n");
	inf.readEoln();
	int a = inf.readInt(1, 10'000);
	inf.readEoln();
	int b = inf.readInt(1, 10'000);
	
	inf.readEof();
}
