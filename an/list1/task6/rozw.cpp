#include <cstdio>
#include <cmath>

const float pi = 3.141592653589793238f;

int main() {

    float approx = 0;
    for (int k = 0; k <= 2000000; k++) {
        if (k % 2 == 0) {
            approx += 1.f / (2.f * (float) k + 1);
        }
        else {
            approx -= 1.f / (2.f * (float) k + 1);
        }
    }

    printf("diff: %.14f\n", fabs(pi - 4 * approx));

    return 0;
}