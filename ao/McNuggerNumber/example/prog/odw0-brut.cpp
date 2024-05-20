#include <bits/stdc++.h>
using namespace std;

int cur = 0;
vector<string> layers;

int main() {
  string s;
  cin >> s;

  layers.push_back("");

  for (char x : s) {
    if (x == '(') {
      layers.push_back("");
    }
    else if (x == ')') {
      string c = layers.back();
      reverse(c.begin(), c.end());
      layers.pop_back();
      layers.back() += c;
    }
    else {
      layers.back().push_back(x);
    }
  }

  cout << layers.back() << "\n";

  return 0;
}
