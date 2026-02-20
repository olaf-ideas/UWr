#include "csapp.h"

static __unused void outc(char c) {
  Write(STDOUT_FILENO, &c, 1);
}

#define N 100
#define M 100

static struct {
  /* TODO: Put semaphores and shared variables here. */
  sem_t mutex;
  int goulash;
  sem_t please_cook;
  sem_t cooking_ended;
} *shared = NULL;


static void savage(void) {
  for (;;) {
    /* TODO Take a meal or wait for it to be prepared. */
    
    Sem_wait(&shared->mutex);
    if (shared->goulash == 0) {
      Sem_post(&shared->please_cook);
      Sem_wait(&shared->cooking_ended);
    }
    assert(shared->goulash > 0);
    outc('E');
    shared->goulash--;

    Sem_post(&shared->mutex);

    /* Sleep and digest. */
    usleep(rand() % 1000 + 1000);
  }

  exit(EXIT_SUCCESS);
}

static void cook(void) {
  for (;;) {
    Sem_wait(&shared->please_cook);
    assert(shared->goulash == 0);
    shared->goulash = M;
    outc('\n');
    Sem_post(&shared->cooking_ended);

    /* TODO Cook is asleep as long as there are meals.
     * If woken up they cook exactly M meals. */
  }
}

/* Do not bother cleaning up after this process. Let's assume that controlling
 * terminal sends SIGINT to the process group on CTRL+C. */
int main(void) {
  shared = Mmap(NULL, getpagesize(), PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED,
                -1, 0);

  /* TODO: Initialize semaphores and other shared state. */
  Sem_init(&shared->mutex, 1, 1);
  shared->goulash = 0;
  Sem_init(&shared->please_cook, 1, 0);
  Sem_init(&shared->cooking_ended, 1, 0);
  
  for (int i = 0; i < N; i++)
    if (Fork() == 0)
      savage();

  cook();

  return EXIT_SUCCESS;
}
