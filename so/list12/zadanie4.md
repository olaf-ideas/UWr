# Zadanie 4

Aktywne czekanie -> while który się kręci dopóty nie otrzymamy dostępu do danych

Blokady usypiające -> zwalniamy blokadę sekcji krytycznej oraz zasypiamy az nie nastanie budzik, który zacznie kontynuuować process

### Czemu oddanie czasu procesora funkcją `yield` nie rozwiązuje wszystkich problemów, które mieliśmy z blokadami wirującymi?

Każdy z czekających (po `yield`) wątków będzie w schedulerze odpytywany zanim proces który zwolni zasób zostanie wywołany. Dużo czasu zmarnowało się na context switch'e.


### Zreferuj implementację podaną w [3,28.14].

```
typedef struct __lock_t {
    int flag;
    int guard;
    queue_t *q;
} lock_t;

void lock_init(lock_t *m) {
    m->flag = 0;
    m->guard = 0;
    queue_init(m->q);
}

void lock(lock_t *m) {
    while (TestAndSet(&m->guard, 1) == 1)
        ;
    if (m->flag == 0) {
        m->flag = 1;
        m->guard = 0;
    } else {
        queue_add(m->q, gettid());
        // setpark();
        m->guard = 0;
        park();
    }
}

void unlock(lock_t *m) {
    while (TestAndSet(&m->guard, 1) == 1)
        ;
    if (queue_empty(m->q))
        m->flag = 0;
    else
        unpark(queue_remove(m->q));
    m->guard = 0;
}
```

### Czemu jest ona niepoprawna bez użycia funkcji `setpark`?

Bez `setpark` podczas wykonywania kodu pomiędzy guard=0 oraz park() może dojść do zmiany wątków. W tym wypadku inny wątek może odblokować oraz zepsuć kolejkę (pierwszy wątek będzie spał wiecznie).