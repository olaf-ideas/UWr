#include <cstdio>
#include <cmath>

const int N = 21;

float I[N];

int main() {

    // I[n] nie powinno byc ujemne a od pewnego momentu spada ponizej 0
    // I[n] tez nie powinno zejsc powyzej 1.

    I[0] = logf(2025.f / 2024.f);
    
    for (int n = 1; n < N; n++) {
        I[n] = 1.f / (float) n - 2024 * I[n - 1];
    }

    for (int n = 0; n < N; n++) {
        printf("%d => %.18f\n", n, I[n]);
    }

    return 0;
}