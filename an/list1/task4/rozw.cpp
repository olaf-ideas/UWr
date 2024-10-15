#include <cstdio>

const int N = 20;

float y[N];

int main() {

    y[0] = 1;
    y[1] = -1 / 6.f;

    // y[n] = (-1 / 6) ^ n
    // wyniki nie dążą do 0 tylko do -inf

    for (int n = 0; n + 2 < N; n++) {
        y[n + 2] = 35.f / 6 * y[n + 1] + y[n];
    }

    for (int n = 0; n < N; n++) {
        printf("%d => %.15f\n", n, y[n]);
    }


    return 0;
}