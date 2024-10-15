#include <cstdio>
#include <cmath>

float f_single(float x) {
    return 1518.f * (2.f * x - sinf(2.f * x)) / (x * x * x);
}

double f_double(double x) {
    return 1518.0 * (2.0 * x - sin(2.0 * x)) / (x * x * x);
}

int main() {

    // lim -> 1518 * 8 / 6 = 2024
    // f(x) powinno dążyć do 2024 a jest zero lub -nan

    for (int i = 10; i <= 20; i++) {
        printf("10^(-%d) => %.18f (single) vs %.18f (double)\n", i, f_single(powf(10.f, (float) -i)), f_double(pow(10.0, (double) -i)));
    }

    return 0;
}