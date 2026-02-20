#include <stdatomic.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
    atomic_int state;
} spin_t;

int compare_and_swap(volatile spin_t *s, int old_val, int new_val) {
    return atomic_compare_exchange_strong(&s->state, &old_val, new_val);
}

void lock(volatile spin_t *s) {
    while (!compare_and_swap(s, 0, 1));
}

void unlock(volatile spin_t *s) {
    atomic_store(&s->state, 0);
}

volatile int cnt = 0;
volatile spin_t s;

void* thread(void*) {
    for (int i = 0; i < 10000; i++) {
        lock(&s);
        cnt++;
        unlock(&s);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }

    atomic_init(&s.state, 0);

    int n = atoi(argv[1]);

    pthread_t tid[n];
    for (int i = 0; i < n; i++) {
        pthread_create(&tid[i], NULL, thread, NULL);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("cnt: %d\n", cnt);

    return 0;
}