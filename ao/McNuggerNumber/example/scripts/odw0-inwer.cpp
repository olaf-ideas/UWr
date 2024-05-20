#include <bits/stdc++.h>
using namespace std;

int main() {
  string s;
  cin >> s;
  assert(s.length() >= 1);
  assert(s.length() <= 1000000);
  int bal = 0;
  for (char x : s) {
    if (x == '(') { bal++; continue; }
    if (x == ')') { bal--; continue; }
    assert(x >= 'a');
    assert(x <= 'z');
  }
  assert(bal == 0);
  return 0;
}
