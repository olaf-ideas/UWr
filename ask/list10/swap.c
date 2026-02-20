extern int printf(const char *, ...);
extern long buf[];

long *bufp0 = &buf[0];
static double sum = 0.0;

__attribute__((section(".bss")))
extern long ext_var;

static void incr() {
  static int count = 0;
  count++;
  sum += 3.14;
  sum += ext_var;
  printf("sum = %f\n", sum);
}

void swap(int i) {
  incr();
  long temp = *bufp0;
  *bufp0 = buf[i];
  buf[i] = temp;
}
