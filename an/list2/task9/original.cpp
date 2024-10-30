#include <iostream>
#include <cmath>

int main() {

	double x = 2;

	double two_k = 1;
	for (int k = 1; k <= 100; k++) {
		printf("k = %d x = %.30f\n", k, x);

		two_k *= 2;

		x = two_k * sqrt(2 * (1 - sqrt(1 - (x / two_k) * (x / two_k))));
	}

}
