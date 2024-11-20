#include <cstdio>
#include <cmath>

const int r = 4;
const float alpha = 80;
const float beta = -700;

float f(float x) {
	return (x - beta) * powf(x - alpha, r);
}

float f_(float x) {
	return pow(x - alpha, r - 1) * (-alpha - beta * r + r * x + x);
}

float fdivf_(float x) {
	return (x - beta) * (x - alpha) / (-alpha - beta * r + r * x + x);
}

float newton_normal() {
	float x = 10, old_x;

	for (int rep = 0; rep < 50 || fabs(f_(x)) < 1e-9; rep++) {
		old_x = x;
		x = x - fdivf_(x); // f(x) / f_(x);
		printf("%d => %.14f\n", rep, x);
		
		if (fabs(x - old_x) < 1e-9)
			break;
	}
	
	printf(" => %.14f\n", x);
	return x;
}

float newton_with_g() {
	float x = 100, old_x;

	for (int rep = 0; rep < 50 || fabs(f_(x)) < 1e-9; rep++) {
		old_x = x;
		x = x - r * fdivf_(x);
		printf("%d => %.14f\n", rep, x);
		
		if (fabs(x - old_x) < 1e-9)
			break;
	}

	printf(" => %.14f\n", x);

	return x;
}

int main() {

	newton_normal();
	puts("----");
	newton_with_g();

	return 0;
}
