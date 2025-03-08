// Olaf Surgut 345615
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void ensure(int test, const char* msg) {
  if (!test) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
  }
}