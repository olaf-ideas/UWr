#include <vector>
#include <utility>
#include <cstdio>
#include <algorithm>

using namespace std;

int n;
vector<pair<int, int>> adj[10007];
vector<vector<int>> wynik;

int odwiedzone[10007];

bool szukaj(int v) {
  if (v == 0 && odwiedzone[v])
    return true;

  odwiedzone[v] = true;
  for (int j = 0; j < adj[v].size(); j++) {
    int u = adj[v][j].second;
    if (odwiedzone[u] == false || u == 0) {
      if (szukaj(u)) {
        wynik.push_back({v, adj[v][j].first, u});
        return true;
      }
    }
  }

  return false;
}

int main() {
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    int l, m, r;
    scanf("%d%d%d", &l, &m, &r);
    adj[l].push_back({m, r});
  }

  szukaj(0);
  if (!wynik.empty()) {
    printf("%d\n", wynik.size());
    
    reverse(wynik.begin(), wynik.end());
    for (int i = 0; i < wynik.size(); i++) {
      for (int j = 0; j < 3; j++)
        printf("%d ", wynik[i][j]);
      printf("\n");
    }
  } else {
    printf("BRAK\n");
  }
}