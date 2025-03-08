#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void ensure(int test, const char* msg) {
  if (!test) {
    fprintf(stderr, "%s\n", msg);
    fprintf(stderr, "error: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}