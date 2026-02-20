#include "csapp.h"

static pid_t spawn(void (*fn)(void)) {
  pid_t pid = Fork();
  if (pid == 0) {
    fn();
    printf("(%d) I'm done!\n", getpid());
    exit(EXIT_SUCCESS);
  }
  return pid;
}

static void grandchild(void) {
  printf("(%d) Waiting for signal!\n", getpid());
  pause();
  printf("(%d) Got the signal!\n", getpid());
}

static void child(void) {
  Setpgid(0, 0);
  pid_t pid = spawn(grandchild);
  printf("(%d) Grandchild (%d) spawned!\n", getpid(), pid);
}

/* Runs command "ps -o pid,ppid,pgrp,stat,cmd" using execve(2). */
static void ps(void) {
  static char *argv[] = {"/usr/bin/ps", "-o", "pid,ppid,pgrp,stat,cmd"};
  execve("/usr/bin/ps", argv, NULL);
}

int main(void) {
  /* TODO: Make yourself a reaper. */
#ifdef LINUX
  Prctl(PR_SET_CHILD_SUBREAPER, 1);
#endif
  printf("(%d) I'm a reaper now!\n", getpid());

  pid_t pid, pgrp;
  int status;

  pgrp = spawn(child); // tworzymy syna (ktory tworzy wnuka)
  printf("(%d) Spawned: %d, now waiting for it to end\n", getpid(), pgrp);
  Waitpid(pgrp, &status, 0); // czekamy az syn sie skonczy

  pid = spawn(ps); // odpalamy rownolegle ps

  Waitpid(pid, &status, 0); // czekamy az ps sie pogrzebie
  printf("(%d) Process ps (%d) ended with status: %d\n", getpid(), pid, status);

  Kill(-pgrp, SIGINT); // zabijamy osieroconego wnuka przez zabicie grupy (syn,wnuk)
  Waitpid(-pgrp, &status, 0); // czekamy az wnuk sie pogrzebie
  printf("(%d) Group %d ended with status: %d\n", getpid(), pgrp, status);


  return EXIT_SUCCESS;
}
