#include <cstdio>
#include <cmath>

float inv_sqrt(float a) {
	float x = 0.0001f; // dla wartosci blisko 0 ladnie zbiega

	for (int rep = 0; rep < 30; rep++) {
		x = x * (3 - a * x * x) * 0.5f;
		printf("%d %.14f\n", rep, x - 1.0f / sqrtf(a));
	}

	printf("%f vs %f\n", x, 1.0f / sqrtf(a));
	return x;
}

int main() {
	
	inv_sqrt(0.5);

	return 0;
}
