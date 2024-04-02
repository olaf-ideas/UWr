#include "oi.h"

using namespace std;

oi::Scanner in(stdin);

const int MAXNM = 2000, MAXC = 50, MAXFV = 1e9;

bool verifyParams(int count, const char* event_type) {
  for (int i = 0; i < count; ++i) {
    int c = in.readInt();
    in.readSpace();
    int f = in.readInt();
    in.readSpace();
    int v = in.readInt();
    in.readEoln();
    if (c < 1 || c > MAXC) {
      printf("ERROR, number of cores in %s number %d = %d\n", event_type, i+1, c);
      return false;
    }
    if (f < 1 || f > MAXFV) {
      printf("ERROR, frequency in %s number %d = %d\n", event_type, i+1, f);
      return false;
    }
    if (v < 1 || v > MAXFV) {
      printf("ERROR, price of %s number %d = %d\n", event_type, i+1, v);
      return false;
    }
  }

  return true;
}

int main() {
  int n = in.readInt();
  in.readEoln();

  if (n < 1 || n > MAXNM) {
    printf("ERROR, n = %d\n", n);
    return 1;
  }

  if (!verifyParams(n, "processor"))
    return 1;

  int m = in.readInt();
  in.readEoln();

  if (m < 1 || m > MAXNM) {
    printf("ERROR, m = %d\n", m);
    return 1;
  }

  if (!verifyParams(m, "order"))
    return 1;
  in.readEof();

  printf("OK, n = %d, m = %d\n", n, m);

  return 0;
}
