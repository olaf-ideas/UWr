/*

Rozwiązanie błędne - brak sortowania po częstotliwości.

*/

#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 2005, MAXC = 52, MAXDP = MAXN * MAXC;
const long long int INF = 1e18;
int n,m;
long long int dp[MAXDP];

void clear() {
  for (int i = 0; i < MAXN*MAXC; ++i)
    dp[i] = -INF;
  dp[0] = 0;
}

int main() {
  clear();
  scanf("%d", &n);
  for (int i = 0; i < n; ++i) {
    int c,f,v;
    scanf("%d%d%d", &c, &f, &v);
    for (int j = MAXDP-1; j >= 0; --j)
      if (j + c < MAXDP && dp[j] != -INF && dp[j+c] < dp[j] - v)
        dp[j+c] = dp[j] - v;
  }
  scanf("%d", &m);
  for (int i = 0; i < m; ++i) {
    int c,f,v;
    scanf("%d%d%d", &c, &f, &v);
    for (int j = 0; j < MAXDP; ++j)
      if (j - c >= 0 && dp[j] != -INF && dp[j-c] < dp[j] + v)
        dp[j-c] = dp[j] + v;
  }

  long long int res = 0;
  for (int i = 0; i < MAXDP; ++i)
    res = max(res, dp[i]);
  printf("%lld\n", res);
}
