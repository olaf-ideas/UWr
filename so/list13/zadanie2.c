#include <pthread.h>
#include <stdio.h>

typedef struct Sem {
    pthread_mutex_t mutex;
    pthread_cond_t waiters;
    int value;
} Sem_t;

void init(Sem_t *s, int init_value) {
    pthread_mutex_init(&s->mutex, NULL);
    pthread_cond_init(&s->waiters, NULL);
    s->value = init_value;
}

void wait(Sem_t *s) {
    pthread_mutex_lock(&s->mutex);
    while (s->value == 0) {
        pthread_cond_wait(&s->waiters, &s->mutex);
    }
    s->value--;
    pthread_mutex_unlock(&s->mutex);
}

void post(Sem_t *s) {
    pthread_mutex_lock(&s->mutex);
    s->value++;
    pthread_mutex_unlock(&s->mutex);
    pthread_cond_signal(&s->waiters);
}

Sem_t s;

int value = 0;

void* thread(void*) {
    wait(&s);

    for (int i = 0; i < 10000000; i++) {
        value++;
    }

    post(&s);

    return NULL;
}

int main() {

    init(&s, 1);
    
    pthread_t t[15];
    for (int i = 0; i < 15; i++) {
        pthread_create(t + i, NULL, thread, NULL);
    }

    for (int i = 0; i < 15; i++) {
        pthread_join(t[i], NULL);
    }

    printf("value = %d\n", value);

    return 0;
}