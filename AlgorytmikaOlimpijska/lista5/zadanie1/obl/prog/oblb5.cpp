/*

  Rozwiązanie błędne - brak long longów.

*/

#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

struct Event {
  enum Type { Processor, Task };
  Type type;
  int cores, frequency, price;
  Event(Type type, int cores, int frequency, int price)
      : type(type), cores(cores), frequency(frequency), price(price) { }
};

const int MAXN = 2005, MAXC = 55, MAXDP = MAXN*MAXC;
int n,m;
vector<Event> events;
int dp[MAXDP];
bool possible[MAXDP];

bool comp(const Event& event1, const Event& event2) {
  if (event1.frequency == event2.frequency)
    return event1.type < event2.type;
  return event1.frequency > event2.frequency;
}

int main() {
  scanf("%d", &n);
  for (int i = 0; i < n; ++i) {
    int c,f,v;
    scanf("%d%d%d", &c, &f, &v);
    events.push_back(Event(Event::Processor,c,f,v));
  }
  scanf("%d", &m);
  for (int i = 0; i < m; ++i) {
    int c,f,v;
    scanf("%d%d%d", &c, &f, &v);
    events.push_back(Event(Event::Task,c,f,v));
  }

  sort(events.begin(), events.end(), comp);

  dp[0] = 0;
  possible[0] = true;

  for (int i = 0; i < (int)events.size(); ++i)
    if (events[i].type == Event::Processor)
      for (int j = MAXDP-1; j >= 0; --j) {
        int index = j + events[i].cores;
        int value = dp[j] - events[i].price;
        if (index < MAXDP && possible[j] && (!possible[index] || dp[index] < value)) {
          dp[index] = value;
          possible[index] = true;
        }
      }
    else
      for (int j = 0; j < MAXDP; ++j) {
        int index = j - events[i].cores;
        int value = dp[j] + events[i].price;
        if (index >= 0 && possible[j] && (!possible[index] || dp[index] < value)) {
          dp[index] = value;
          possible[index] = true;
        }
      }

  int result = 0;
  for (int i = 0; i < MAXDP; ++i)
    if (possible[i])
      result = max(result, dp[i]);

  printf("%d\n", result);
}
