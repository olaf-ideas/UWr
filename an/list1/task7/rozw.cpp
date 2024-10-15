#include <cstdio>
#include <cmath>

float my_sin(float x) {
    float sum = 0;

    float a = x;
    // a_n = x^(2n + 1) / (2n + 1)!

    for (int n = 0; a > 1e-6f; n++) {
        if (n % 2 == 0) {
            sum += a;
        }
        else {
            sum -= a;
        }

        a *= x;
        a *= x;
        a /= 2.f * (float) n + 2;
        a /= 2.f * (float) n + 3;
    }

    return sum;
}

int main() {

    float tau = 2 * acosf(-1);

    for (int i = 0; i <= 100; i++) {
        float x = tau * (float) i / 100;

        printf("sin(%.5f): %.14f vs %.14f (error %.14f)\n", x, sinf(x), my_sin(x), fabsf(sinf(x) - my_sin(x)));
    }

    return 0;
}
