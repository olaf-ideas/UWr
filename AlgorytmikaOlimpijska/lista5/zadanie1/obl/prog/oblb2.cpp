/*

  Rozwiązanie błędne - metoda zachłanna oparta na opłacalności
  w przeliczeniu na 1 rdzeń. Ignoruje kompletnie częstotliwości.

*/

#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

struct Processor {
  int cores, freq, value;
  Processor(int cores, int freq, int value)
      : cores(cores), freq(freq), value(value) { }
};

struct Task {
  int cores, freq, value;
  Task(int cores, int freq, int value)
      : cores(cores), freq(freq), value(value) { }
};

const int MAXN = 2005, MAXC = 52;
const long long int INF = 1e18;
int n,m;
vector<Processor> procs;
vector<Task> tasks;
long long int budget;

bool compTask(const Task& t1, const Task& t2) {
  double coeff1 = ((double)t1.value) / t1.cores;
  double coeff2 = ((double)t2.value) / t2.cores;
  return coeff1 > coeff2;
}

bool compProc(const Processor& p1, const Processor& p2) {
  double coeff1 = ((double)p1.value) / p1.cores;
  double coeff2 = ((double)p2.value) / p2.cores;
  return coeff1 < coeff2;
}

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
    tasks.push_back(Task(c,f,v));
  }

  sort(tasks.begin(), tasks.end(), compTask);
  sort(procs.begin(), procs.end(), compProc);

  long long int res = 0, current = 0;
  int cores = 0, itr_tasks = 0;
  for (int i = 0; i < (int)procs.size() && itr_tasks < (int)tasks.size(); ++i) {
    current -= procs[i].value;
    cores += procs[i].cores;
    while (itr_tasks < (int)tasks.size() && tasks[itr_tasks].cores <= cores) {
      current += tasks[itr_tasks].value;
      cores -= tasks[itr_tasks].cores;
      itr_tasks++;
    }
    res = max(res, current);
  }

  printf("%lld\n", res);
}
