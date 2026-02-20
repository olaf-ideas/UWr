#include "csapp.h"

static __unused void outc(char c) {
  Write(STDOUT_FILENO, &c, 1);
}

static __thread unsigned seed;

static sem_t tobacco;
static sem_t matches;
static sem_t paper;
static sem_t doneSmoking;
/* TODO: If you need any extra global variables, then define them here. */
static sem_t mutex;
static int tobacco_flag;
static sem_t tobaccoSmoker;
static int matches_flag;
static sem_t matchesSmoker;
static int paper_flag;
static sem_t paperSmoker;

static void *agent(void *arg) {
  seed = pthread_self();

  while (true) {
    Sem_wait(&doneSmoking);

    int choice = rand_r(&seed) % 3;
    if (choice == 0) {
      // outc('m');
      Sem_post(&tobacco);
      Sem_post(&paper);
    } else if (choice == 1) {
      // outc('p');
      Sem_post(&tobacco);
      Sem_post(&matches);
    } else {
      // outc('t');
      Sem_post(&paper);
      Sem_post(&matches);
    }
  }

  return NULL;
}

// wait tobacco


// wait matches
// if get_vlaue paper == 2 -> git

/* TODO: If you need extra threads, then define their main procedures here. */
static void* tobaccoObserver(void *arg) {
  while (true) {
    Sem_wait(&tobacco);

    Sem_wait(&mutex);
    if (matches_flag) {
      matches_flag = 0;
      Sem_post(&paperSmoker);
    } else if (paper_flag) {
      paper_flag = 0;
      Sem_post(&matchesSmoker);
    } else {
      tobacco_flag = 1;
    }
    Sem_post(&mutex);
  }

  return NULL;
}

static void* matchesObserver(void *arg) {
  while (true) {
    Sem_wait(&matches);

    Sem_wait(&mutex);
    if (tobacco_flag) {
      tobacco_flag = 0;
      Sem_post(&paperSmoker);
    } else if (paper_flag) {
      paper_flag = 0;
      Sem_post(&tobaccoSmoker);
    } else {
      matches_flag = 1;
    }
    Sem_post(&mutex);
  }

  return NULL;
}

static void* paperObserver(void *arg) {
  while (true) {
    Sem_wait(&paper);

    Sem_wait(&mutex);
    if (tobacco_flag) {
      tobacco_flag = 0;
      Sem_post(&matchesSmoker);
    } else if (matches_flag) {
      matches_flag = 0;
      Sem_post(&tobaccoSmoker);
    } else {
      paper_flag = 1;
    }
    Sem_post(&mutex);
  }

  return NULL;
}

static void randsleep(void) {
  usleep(rand_r(&seed) % 1000 + 1000);
}

static void make_and_smoke(char smoker) {
  randsleep();
  Sem_post(&doneSmoking);
  outc(smoker);
  randsleep();
}

static void *smokerWithMatches(void *arg) {
  seed = pthread_self();

  while (true) {
    /* TODO: wait for paper and tobacco */
    Sem_wait(&matchesSmoker);
    make_and_smoke('M');
  }

  return NULL;
}

static void *smokerWithTobacco(void *arg) {
  seed = pthread_self();

  while (true) {
    /* TODO: wait for paper and matches */
    Sem_wait(&tobaccoSmoker);
    make_and_smoke('T');
  }

  return NULL;
}

static void *smokerWithPaper(void *arg) {
  seed = pthread_self();
 
  while (true) {
    /* TODO: wait for tobacco and matches */
    Sem_wait(&paperSmoker);
    make_and_smoke('P');
  }

  return NULL;
}

int main(void) {
  Sem_init(&tobacco, 0, 0);
  Sem_init(&matches, 0, 0);
  Sem_init(&paper, 0, 0);
  Sem_init(&doneSmoking, 0, 1);

  /* TODO: Initialize your global variables here. */
  Sem_init(&mutex, 0, 1);
  Sem_init(&tobaccoSmoker, 0, 0);
  Sem_init(&matchesSmoker, 0, 0);
  Sem_init(&paperSmoker, 0, 0);
  tobacco_flag = 0;
  matches_flag = 0;
  paper_flag = 0;

  pthread_t tobaccoObserverThread, matchesObserverThread, paperObserverThread;
  Pthread_create(&tobaccoObserverThread, NULL, tobaccoObserver, NULL);
  Pthread_create(&matchesObserverThread, NULL, matchesObserver, NULL);
  Pthread_create(&paperObserverThread, NULL, paperObserver, NULL);

  pthread_t agentThread;
  Pthread_create(&agentThread, NULL, agent, NULL);

  pthread_t smokerPaperThread, smokerMatchesThread, smokerTobaccoThread;
  Pthread_create(&smokerPaperThread, NULL, smokerWithPaper, NULL);
  Pthread_create(&smokerMatchesThread, NULL, smokerWithMatches, NULL);
  Pthread_create(&smokerTobaccoThread, NULL, smokerWithTobacco, NULL);

  Pthread_join(tobaccoObserverThread, NULL);
  Pthread_join(matchesObserverThread, NULL);
  Pthread_join(paperObserverThread, NULL);

  Pthread_join(agentThread, NULL);
  Pthread_join(smokerPaperThread, NULL);
  Pthread_join(smokerMatchesThread, NULL);
  Pthread_join(smokerTobaccoThread, NULL);

  return 0;
}
