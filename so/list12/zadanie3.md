# Zadanie 3

### Semantyka instrukcji atomowej `compare-and-swap`

W C++ std::atomic_compare_exchange_weak/strong

Nie atomowa implementaca:
```
__atomic
int compare_and_swap(int *ptr, int old, int new) {
    if (*ptr != old)
        return false;
    *ptr = new;
    return true;
}
```

### Zaimplementuj blokadę wirującą (spin lock)

./zadanie3.c

### Czemu blokada wirująca nie jest sprawiedliwa

Jeśli scheduler popełni błąd to wątek który został uruchomiony,
albo kontynuuje pracę lub czeka przez kręcenie się w miejscu.
To oznacza brak prewencji przed zagłodzeniem i duże marnotractwo
zasobów.

### N indentycznych wątków, ile czasu zajmie wszystkim wątkom jednoktorne przejscie przez sekcję krytyczną?

1, 2, 3, ..., N

1 -> lock 
2 -> spin
3 -> spin
..
n -> spin
1 -> unlock
2 -> lock
3 -> spin
4 -> spin
...
n -> spin
2 -> unlock
3 -> spin

...

O(n^2) kwantów czasu na wykonanie wątków.