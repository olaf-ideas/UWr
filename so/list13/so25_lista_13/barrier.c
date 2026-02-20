#include "csapp.h"

static __unused void outc(char c) {
  Write(STDOUT_FILENO, &c, 1);
}

typedef struct {
  /* TODO: Use this structure to store barrier internal state. */
  sem_t mutex;
  int barrier_size;
  int waiting;

  sem_t start;
  sem_t next_batch;
} barrier_t;

static barrier_t *barrier_init(int n) {
  if (n < 1)
    app_error("barrier_init requires n > 0");

  barrier_t *b = Mmap(NULL, sizeof(barrier_t), PROT_READ|PROT_WRITE,
                      MAP_ANON|MAP_SHARED, -1, 0);

  /* TODO: Initialize barrier internal state. */
  Sem_init(&b->mutex, 1, 1);
  b->barrier_size = n;
  b->waiting = 0;

  Sem_init(&b->start, 1, 0);
  Sem_init(&b->next_batch, 1, 1);
  return b;
}

static void barrier_wait(barrier_t *b) {
  /* TODO: Provide wait procedure implementation here. */
  Sem_wait(&b->next_batch);
  // outc('(');
  // Sem_wait(&b->mutex);
  b->waiting++;
  if (b->waiting == b->barrier_size) {
    Sem_post(&b->start);
  }
  else {
    Sem_post(&b->next_batch);
  }
  // Sem_post(&b->mutex);

  Sem_wait(&b->start);
  Sem_post(&b->start);
  
  Sem_wait(&b->mutex);
  b->waiting--;
  // outc((char)(b->waiting + '0'));
  outc(')');
  if (b->waiting == 0) {
    outc('\n');
    Sem_wait(&b->start);
    Sem_post(&b->next_batch);
  }
  Sem_post(&b->mutex);
}

static void barrier_destroy(barrier_t *b) {
  /* TODO: Provide destroy procedure implementation here. */
  Sem_destroy(&b->next_batch);
  Sem_destroy(&b->start);
  Sem_destroy(&b->mutex);

  Munmap(b, sizeof(barrier_t));
}

#define K 100
#define N 50
#define P 100

static noreturn void horse(barrier_t *b) {
  int n = rand() % K + K;

  // outc('+');

  for (int i = 0; i < n; i++) {
    barrier_wait(b);
    usleep(rand() % 2000 + 1000);
  }

  // outc('-');

  exit(EXIT_SUCCESS);
}

/* Do not bother cleaning up after this process. Let's assume that controlling
 * terminal sends SIGINT to the process group on CTRL+C. */
int main(void) {
  barrier_t *b = barrier_init(N);

  int horses = 0;

  for (;;) {
    do {
      if (Fork() == 0) {
        srand(getpid());
        horse(b);
      }
      horses++;
    } while (horses < P);

    Wait(NULL);
    horses--;
  }

  barrier_destroy(b);

  return EXIT_SUCCESS;
}
