#include <bits/stdc++.h>
#include "oi.h"
using namespace std;

oi::Random rg;

struct Processor {
  int cores, freq, price;
  Processor(int cores_, int freq_, int price_):
    cores(cores_), freq(freq_), price(price_) {}
};

struct Task {
  int cores, freq, reward;
  Task(int cores_, int freq_, int reward_):
    cores(cores_), freq(freq_), reward(reward_) {}
};

void Test(const string& name,
          int seed,
          const vector<Processor>& processors,
          const vector<Task>& tasks) {
  rg.setSeed(seed);
  FILE* f = fopen(name.c_str(), "w");
  fprintf(f, "%d\n", (int)processors.size());
  for (const Processor& proc : processors)
    fprintf(f, "%d %d %d\n", proc.cores, proc.freq, proc.price);
  fprintf(f, "%d\n", (int)tasks.size());
  for (const Task& task : tasks)
    fprintf(f, "%d %d %d\n", task.cores, task.freq, task.reward);
  fclose(f);
}

void Test(const string& name,
          int seed,
          const pair<vector<Processor>,vector<Task>>& data) {
  Test(name, seed, data.first, data.second);
}

vector<Processor> SimpleGenProcessors(
  int n,
  int min_cores, int max_cores,
  int min_freq, int max_freq,
  int min_price, int max_price) {

  vector<Processor> processors;
  for (int i = 0; i < n; ++i) {
    int cores = rg.rand() % (max_cores - min_cores + 1) + min_cores;
    int freq = rg.rand() % (max_freq - min_freq + 1) + min_freq;
    int price = rg.rand() % (max_price - min_price + 1) + min_price;
    processors.push_back(Processor(cores, freq, price));
  }

  return processors;
}

vector<Processor> GenProcessors(
  int n,
  int min_rnd, int max_rnd,
  double exp_freq, double exp_cores,
  int min_cores, int max_cores,
  int min_freq, int max_freq,
  int max_price) {

  vector<Processor> processors;

  while ((int)processors.size() < n) {
    int rnd = rg.rand() % (max_rnd - min_rnd + 1) + min_rnd;
    int cores = rg.rand() % (max_cores - min_cores + 1) + min_cores;
    int freq = rg.rand() % (max_freq - min_freq + 1) + min_freq;

    double log_cores = log10(cores);
    double log_freq = log10(freq);

    double price = pow(exp_cores, log_cores) * pow(exp_freq, log_freq) * rnd;
    if (price > max_price) continue;
    if (price == 0) continue;
    processors.push_back(Processor(cores, freq, (int)price));
  }

  return processors;
}

vector<Task> SimpleGenTasks(
  int m,
  int min_cores, int max_cores,
  int min_freq, int max_freq,
  int min_price, int max_price) {

  vector<Task> tasks;
  for (int i = 0; i < m; ++i) {
    int cores = rg.rand() % (max_cores - min_cores + 1) + min_cores;
    int freq = rg.rand() % (max_freq - min_freq + 1) + min_freq;
    int price = rg.rand() % (max_price - min_price + 1) + min_price;
    tasks.push_back(Task(cores, freq, price));
  }

  return tasks;
}

vector<Task> GenTasks(
  int m,
  int min_rnd, int max_rnd,
  double exp_freq, double exp_cores,
  int min_cores, int max_cores,
  int min_freq, int max_freq,
  int max_reward) {

  vector<Task> tasks;

  while ((int)tasks.size() < m) {
    int rnd = rg.rand() % (max_rnd - min_rnd + 1) + min_rnd;
    int cores = rg.rand() % (max_cores - min_cores + 1) + min_cores;
    int freq = rg.rand() % (max_freq - min_freq + 1) + min_freq;

    double log_cores = log10(cores);
    double log_freq = log10(freq);

    double reward = pow(exp_cores, log_cores) * pow(exp_freq, log_freq) * rnd;
    if (reward > max_reward) continue;
    if (reward == 0) continue;
    tasks.push_back(Task(cores, freq, (int)reward));
  }

  return tasks;
}

// Jeśli f jest rosnąca, to nie opłaca się kupić żadnego procesora
// zatem odpowiedź dla testu to 0.
pair<vector<Processor>, vector<Task>> GenFreqPriceProportional(
  int n, int m,
  int min_cores, int max_cores,
  int min_freq, int max_freq,
  int max_price,
  function<long long int(long long int)> f) {

  unordered_set<int> used_freqs, used_prices;
  vector<Task> tasks;
  vector<Processor> procs;

  while ((int)tasks.size() < m || (int)procs.size() < n) {
    int cores = rg.rand() % (max_cores - min_cores + 1) + min_cores;
    int freq = rg.rand() % (max_freq - min_freq + 1) + min_freq;
    int event_type = rg.rand() % 2;
    int price = f(freq);
    if (price > max_price) continue;
    if (used_freqs.count(freq) > 0) continue;
    if (used_prices.count(price) > 0) continue;
    used_freqs.insert(freq);
    used_prices.insert(price);
    if ((int)procs.size() == n || event_type == 0)
      tasks.push_back(Task(cores, freq, price));
    else
      procs.push_back(Processor(cores, freq, price));
  }

  return make_pair(procs, tasks);
}

int main() {
  // Subtask 1
  printf("obl1a...\n");
  Test("obl1a.in", 1,
       SimpleGenProcessors(1, 3, 4, 3001, 4000, 500, 1000),
       SimpleGenTasks(1, 1, 2, 2000, 3000, 500, 1000));
  printf("obl1b...\n");
  Test("obl1b.in", 2,
       SimpleGenProcessors(1, 1, 2, 2000, 3000, 500, 1000),
       SimpleGenTasks(1, 3, 4, 3001, 4000, 500, 1000));
  printf("obl1c...\n");
  Test("obl1c.in", 3,
       SimpleGenProcessors(8, 1, 20, 5000, 7000, 1, 10000),
       SimpleGenTasks(100, 1, 3, 3000, 5500, 1, 10000));
  printf("obl1d...\n");
  Test("obl1d.in", 4,
       GenProcessors(7, 1, 5, 15.0, 15.0, 1, 20, 5000, 7000, 1e7),
       GenTasks(200, 1, 5, 15.0, 15.0, 1, 3, 3000, 5500, 1e7));
  printf("obl1e...\n");
  Test("obl1e.in", 5,
       SimpleGenProcessors(15, 1, 50, 1e7, 1e9, 1, 1e5),
       SimpleGenTasks(1900, 1, 50, 1e6, 1e9, 1, 1e5));
  printf("obl1f...\n");
  Test("obl1f.in", 6,
       GenProcessors(15, 1, 5, 13.0, 13.0, 1, 50, 1e7, 1e9, 1e9),
       GenTasks(1903, 1, 5, 13.0, 13.0, 1, 50, 1e6, 1e9, 1e9));
  printf("obl1g...\n");
  Test("obl1g.in", 7,
       SimpleGenProcessors(15, 45, 50, 1e8, 1e9, 1e8, 1e9),
       SimpleGenTasks(2000, 45, 50, 1e8, 1e9, 1e8, 1e9));
  printf("obl1h...\n");
  Test("obl1h.in", 8,
       GenProcessors(15, 1, 2, 10.0, 10.0, 40, 50, 1e7, 1e8, 1e9),
       GenTasks(2000, 1, 2, 10.0, 10.0, 40, 50, 1e7, 1e8, 1e9));

  // Subtask 2
  printf("obl2a...\n");
  Test("obl2a.in", 20,
       SimpleGenProcessors(1, 3, 4, 3001, 4000, 1000, 2000),
       SimpleGenTasks(2, 1, 2, 2000, 3000, 1000, 2000));
  // Odpowiedź to 0.
  printf("obl2b...\n");
  Test("obl2b.in", 8, GenFreqPriceProportional(5, 5, 1, 1, 1, 15, 5000, [](long long int x) { return 2*x+3; }));
  printf("obl2c...\n");
  Test("obl2c.in", 21,
       SimpleGenProcessors(100, 1, 5, 3000, 5000, 200, 1000),
       SimpleGenTasks(6, 20, 40, 2500, 4500, 200, 1000));
  printf("obl2d...\n");
  Test("obl2d.in", 22,
       GenProcessors(100, 1, 10, 15.0, 15.0, 1, 5, 3000, 5000, 1e6),
       GenTasks(6, 1, 10, 15.0, 15.0, 20, 40, 2500, 4500, 1e6));
  printf("obl2e...\n");
  Test("obl2e.in", 23,
       SimpleGenProcessors(900, 1, 50, 1e4, 1e5, 1e4, 1e5),
       SimpleGenTasks(13, 20, 40, 1e4, 5*1e4, 1e4, 1e7));
  printf("obl2f...\n");
  Test("obl2f.in", 24,
       GenProcessors(900, 1, 5, 13.0, 13.0, 1, 50, 1e4, 1e5, 1e5),
       GenTasks(13, 1, 5, 13.0, 13.0, 20, 40, 1e4, 5*1e4, 1e7));
  printf("obl2g...\n");
  Test("obl2g.in", 25,
       SimpleGenProcessors(2000, 1, 50, 1e7, 1e9, 1e7, 1e9),
       SimpleGenTasks(15, 40, 50, 1e7, 1e9, 1e7, 1e9));
  printf("obl2h...\n");
  Test("obl2h.in", 26,
       GenProcessors(2000, 1, 5, 11.0, 11.0, 1, 50, 1e7, 1e9, 1e9),
       GenTasks(15, 1, 5, 11.0, 11.0, 40, 50, 1e7, 1e9, 1e9));
  printf("obl2i...\n");
  Test("obl2i.in", 27,
       SimpleGenProcessors(1937, 40, 50, 1, 1, 1, 20),
       SimpleGenTasks(15, 40, 50, 1, 1, 1e8, 1e9));

  //Subtask 3
  printf("obl3a...\n");
  Test("obl3a.in", 30,
       SimpleGenProcessors(12, 1, 1, 2000, 5000, 500, 4000),
       SimpleGenTasks(18, 1, 1, 2000, 5000, 2000, 5000));
  printf("obl3b...\n");
  Test("obl3b.in", 31,
       GenProcessors(12, 1, 3, 15.0, 15.0, 1, 1, 2000, 5000, 9000),
       GenTasks(18, 1, 3, 15.0, 15.0, 1, 1, 2000, 5000, 13000));
  printf("obl3c...\n");
  Test("obl3c.in", 32,
       SimpleGenProcessors(632, 1, 1, 2000, 5000, 1000000, 5000000),
       SimpleGenTasks(800, 1, 1, 4000, 8000, 1000000, 5000000));
  printf("obl3d...\n");
  Test("obl3d.in", 33,
       GenProcessors(632, 1, 4, 14.0, 14.0, 1, 1, 2000, 5000, 5000000),
       GenTasks(800, 1, 4, 14.0, 14.0, 1, 1, 4000, 8000, 5000000));
  printf("obl3e...\n");
  Test("obl3e.in", 34,
       SimpleGenProcessors(1794, 1, 1, 4000, 8000, 1e7, 1e9),
       SimpleGenTasks(1113, 1, 1, 2000, 5000, 1e7, 1e9));
  printf("obl3f...\n");
  Test("obl3f.in", 35,
       GenProcessors(1794, 1, 5, 13.0, 13.0, 1, 1, 4000, 8000, 1e9),
       GenTasks(1113, 1, 5, 13.0, 13.0, 1, 1, 2000, 5000, 1e9));
  printf("obl3g...\n");
  Test("obl3g.in", 36,
       SimpleGenProcessors(2000, 1, 1, 1e8, 1e9, 1e7, 1e9),
       SimpleGenTasks(2000, 1, 1, 1e8, 1e9, 1e7, 1e9));
  printf("obl3h...\n");
  Test("obl3h.in", 37,
       GenProcessors(2000, 1, 2, 12.0, 12.0, 1, 1, 1e8, 1e9, 1e9),
       GenTasks(2000, 1, 2, 12.0, 12.0, 1, 1, 1e8, 1e9, 1e9));

  //Subtask 4
  printf("obl4a...\n");
  Test("obl4a.in", 40,
       SimpleGenProcessors(10, 1, 5, 1, 1, 500, 4000),
       SimpleGenTasks(6, 1, 5, 1, 1, 2000, 5000));
  printf("obl4b...\n");
  Test("obl4b.in", 41,
       GenProcessors(10, 2, 4, 15.0, 15.0, 1, 5, 1, 1, 4000),
       GenTasks(6, 2, 4, 15.0, 15.0, 1, 5, 1, 1, 5000));
  printf("obl4c...\n");
  Test("obl4c.in", 42,
       SimpleGenProcessors(1500, 1, 5, 1, 1, 500, 1000),
       SimpleGenTasks(7, 40, 50, 1, 1, 2000, 5000));
  printf("obl4d...\n");
  Test("obl4d.in", 43,
       SimpleGenProcessors(13, 1, 50, 1, 1, 5000, 12000),
       SimpleGenTasks(1805, 1, 10, 1, 1, 200, 1000));
  printf("obl4e...\n");
  Test("obl4e.in", 44,
       SimpleGenProcessors(1992, 1, 50, 1, 1, 1e7, 1e9),
       SimpleGenTasks(1772, 1, 50, 1, 1, 1e7, 1e9));
  printf("obl4f...\n");
  Test("obl4f.in", 45,
       SimpleGenProcessors(2000, 40, 50, 1, 1, 1e7, 1e9),
       SimpleGenTasks(2000, 40, 50, 1, 1, 1, 20));
  printf("obl4g...\n");
  Test("obl4g.in", 46,
       SimpleGenProcessors(2000, 40, 50, 1, 1, 1, 20),
       SimpleGenTasks(2000, 40, 50, 1, 1, 1e8, 1e9));
  printf("obl4h...\n");
  Test("obl4h.in", 47,
       SimpleGenProcessors(1937, 40, 50, 1, 1, 1, 20),
       SimpleGenTasks(1984, 40, 50, 1, 1, 1e8, 1e9));

  //Subtask 5
  printf("obl5a...\n");
  Test("obl5a.in", 50,
       SimpleGenProcessors(4, 40, 50, 2000, 4000, 1, 1),
       SimpleGenTasks(2, 40, 50, 2000, 4000, 1, 1));
  printf("obl5b...\n");
  Test("obl5b.in", 51,
       SimpleGenProcessors(57, 40, 50, 1000, 1500, 1, 1),
       SimpleGenTasks(102, 40, 50, 2000, 4000, 1, 1));
  printf("obl5c...\n");
  Test("obl5c.in", 52,
       SimpleGenProcessors(570, 40, 50, 3000, 10000, 1, 1),
       SimpleGenTasks(246, 1, 4, 3000, 10000, 1, 1));
  printf("obl5d...\n");
  Test("obl5d.in", 53,
       SimpleGenProcessors(800, 1, 4, 10000, 100000, 1, 1),
       SimpleGenTasks(1200, 1, 4, 3000, 10000, 1, 1));
  printf("obl5e...\n");
  Test("obl5e.in", 54,
       SimpleGenProcessors(1968, 40, 50, 1, 1e9, 1, 1),
       SimpleGenTasks(1996, 40, 50, 1, 1e9, 1, 1));
  printf("obl5f...\n");
  Test("obl5f.in", 55,
       SimpleGenProcessors(2000, 40, 50, 2, 2, 1, 1),
       SimpleGenTasks(2000, 45, 50, 1, 1, 1, 1));


  //Subtask 6
  printf("obl6a...\n");
  Test("obl6a.in", 60,
       GenFreqPriceProportional(503, 476, 40, 50, 1, 5000, 1e9, [](long long int x) { return x*x; }));
  printf("obl6b...\n");
  Test("obl6b.in", 61,
       GenFreqPriceProportional(756, 1174, 40, 50, 1, 5000, 1e9, [](long long int x) { return x + (rg.rand() % (max(1LL, x/10))); }));
  printf("obl6c...\n");
  Test("obl6c.in", 62,
       GenFreqPriceProportional(1534, 1383, 1, 1, 1, 10000, 1e9, [](long long int x) { return (x < 5000) ? 2*x+30 : 100000-3*(x-5000); }));
  // Test gdzie trzeba kupić wszystkie procesory, a każdy z nich ma max liczbę rdzeni.
  printf("obl6d...\n");
  Test("obl6d.in", 63,
       SimpleGenProcessors(2000, 50, 50, 4001, 5000, 100, 1000),
       SimpleGenTasks(2000, 50, 50, 3000, 4000, 1001, 2000));
  // Test na long longi
  printf("obl6e...\n");
  Test("obl6e.in", 64,
       SimpleGenProcessors(2000, 50, 50, 4001, 5000, 1, 10),
       SimpleGenTasks(2000, 50, 50, 3000, 4000, 1e9, 1e9));
  // Maxtest
  printf("obl6f...\n");
  Test("obl6f.in", 65,
       SimpleGenProcessors(2000, 50, 50, 1e8, 1e9, 1e8, 1e9),
       SimpleGenTasks(2000, 50, 50, 1e8, 1e9, 1e8, 1e9));
  printf("obl6g...\n");
  Test("obl6g.in", 66,
       GenFreqPriceProportional(1000, 1000, 1, 1, 1, 5000, 50000, [](long long int x) { return 2*x+3; }));
  printf("obl6h...\n");
  Test("obl6h.in", 55,
       GenFreqPriceProportional(1503, 1476, 1, 1, 1, 5000, 1e9, [](long long int x) { return x*x; }));
  printf("obl6i...\n");
  Test("obl6i.in", 56,
       GenFreqPriceProportional(1507, 1438, 1, 1, 1, 1e6, 1e9, [](long long int x) { return 1e9/x; }));

  return 0;
}

