#include <bits/stdc++.h>
using namespace std;

int bal = 0;
bool mode = true;

int main(int argc, char* argv[]) {
  int n = atoi(argv[1]), depth = atoi(argv[2]), perc = atoi(argv[3]), sr = atoi(argv[4]);

  srand(sr);

  for (int i = 0; i < n; i++) {
    int remaining_chars = n - i;

    bool letter = false;
    if ((bal < remaining_chars) && (rand() % 100 >= perc))
      letter = true;

    if (letter)
      cout << (char)(rand() % ('z' - 'a' + 1) + 'a');
    else {
      bool opening = false;
      if ((bal + 1 <= depth) && (bal + 1 <= remaining_chars)) {
        int prob = (mode) ? 70 : 30;
        if (rand() % 100 < prob)
          opening = true;
      }
      if ((bal == 0) && (!opening)) {
        i--;
        continue;
      }

      if (opening) {
        cout << "(";
        bal++;

        if (bal == depth)
          mode = false;
      }
      else {
        cout << ")";
        bal--;

        if (bal == 0)
          mode = true;
      }
    }
  }

  cout << "\n";

  return 0;
}
