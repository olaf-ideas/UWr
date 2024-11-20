#include <cstdio>
#include <cmath>

float my_inv(float r) {
	float x = 1, old;

	int cnt = 0;
	do {
		old = x;
		x = x * (2 - x * r);
		cnt++;
	
		printf("%f
	} while (fabs(x - old) > 1e-6f);

	printf("cnt = %d\n", cnt);

	return x;
}

int main() {
	
	printf("%.14f\n", my_inv(10));

	return 0;
}
