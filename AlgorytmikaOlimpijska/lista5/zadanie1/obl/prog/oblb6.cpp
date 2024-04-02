/*

  Rozwiązanie błędne - metoda zachłanna do podzadania 5. Jeśli
  każdy procesor kosztuje tyle samo, to zaczyna od tych o najwyższej
  częstotliwości (w drugiej kolejności patrzy na liczbę rdzeni).
  Zadanie jest wybierane biorąc lower_bound na częstotliwościach
  zadań (zadanie o najwyższej częstotliwości, które da się wykonać).

*/

#include <cstdio>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

struct Processor {
  int cores, freq, value;
  Processor(int cores, int freq, int value)
      : cores(cores), freq(freq), value(value) { }
};

struct Task {
  int cores, freq, value;
  Task() : cores(1), freq(1), value(1) { }
  Task(int cores, int freq, int value)
      : cores(cores), freq(freq), value(value) { }
  bool operator() (const Task& t1, const Task& t2) {
    if (t1.freq == t2.freq) return t1.cores > t2.cores;
    return t1.freq > t2.freq;
  }
};

bool compProc(const Processor& p1, const Processor& p2) {
  if (p1.freq == p2.freq) return p1.cores > p2.cores;
  return p1.freq > p2.freq;
}

const int MAXN = 2005, MAXC = 52;
const long long int INF = 1e18;
int n,m;
vector<Processor> procs;
set<Task, Task> tasks;

int main() {
  scanf("%d", &n);
  for (int i = 0; i < n; ++i) {
    int c,f,v;
    scanf("%d%d%d", &c, &f, &v);
    procs.push_back(Processor(c,f,v));
  }
  scanf("%d", &m);
  for (int i = 0; i < m; ++i) {
    int c,f,v;
    scanf("%d%d%d", &c, &f, &v);
    tasks.insert(Task(c,f,v));
  }

  sort(procs.begin(), procs.end(), compProc);

  long long int res = 0, current = 0;
  int cores = 0;
  for (int i = 0; i < (int)procs.size() && (int)tasks.size() > 0; ++i) {
    current -= procs[i].value;
    cores += procs[i].cores;
    set<Task, Task>::iterator lower_bound = tasks.lower_bound(Task(51, procs[i].freq, 1));
    while(lower_bound != tasks.end() && lower_bound->cores >= cores) {
      cores -= lower_bound->cores;
      current += lower_bound->value;
      tasks.erase(lower_bound);
      lower_bound = tasks.lower_bound(Task(51, procs[i].freq, 1));
    }
    res = max(res, current);
  }

  printf("%lld\n", res);
}
