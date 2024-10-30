#include <cstdio>
#include <cmath>

float a(float x) {
	return 1.0f / (pow(x, 5) + sqrt(pow(x, 10) + 2024));
}

float a_fixed(float x) {
	return - (pow(x, 5) - sqrt(pow(x, 10) + 2024)) / 2024;
}

float b(float x) {
	return 1e8f * (exp(x) - exp(2 * x));
}

float b_fixed(float x) {
	float res = 0;
	for (int k = 30; k >= 0; k--) {
		res *= x / (k + 1);
		res += (1 - pow(2, k));
	}
	return res * 1e8f;
}

float c(float x) {
	return 6 * pow(x, -3) * (asin(x) - x);
}

float c_fixed(float x) {
	return 1 + 18 / 40 * pow(x, 2) + 30 / 112 * pow(x, 4);
}

float d(float x) {
	return 4 * pow(cos(x), 2) - 1;
}

#define float double

const float pi = 3.141592653589793238462643383279;

float d_fixed(float x) {
	return (-sqrt(3) * (x - pi / 3) - 0.5 * pow(x - pi / 3, 2) + pow(x - pi / 3, 3) / (2 * sqrt(3))) * (2 * cos(x) + 1);
}

int main() {

	printf("%.20f\n", a(-100));
	printf("%.20f\n", a_fixed(-100));
	
	printf("%.20f\n", b(1e-17));
	printf("%.20f\n", b_fixed(1e-17));

	printf("%.20f\n", c(1e-17));
	printf("%.20f\n", c_fixed(1e-17));

	const float x = pi / 3 + 1e-8L;

	printf("%.20f\n", -sqrt(3) * (x - pi / 3) - 0.5 * pow(x - pi / 3, 2));
	printf("x = %.20f\n", x);
	printf("c = %.20f\n", cos(x));

	printf("%.20f\n", d(x));
	printf("%.20f\n", d_fixed(x));

	return 0;
}
