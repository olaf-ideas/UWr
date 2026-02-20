#include "csapp.h"

static int ndselect(int n) {
  /* TODO: A loop is missing here that spawns processes and waits for them! */
	while (n --> 1) {
		int child_pid = Fork();
		if (child_pid == 0) // we are a child
			break;
		Waitpid(child_pid, NULL, 0);
	}

	return n;
}

static int conflict(int x1, int y1, int x2, int y2) {
  return x1 == x2
    || y1 == y2
    || x1 + y1 == x2 + y2
    || x1 - y1 == x2 - y2;
}

static void print_line_sep(int size) {
  for (int i = 0; i < size; ++i) 
    printf("+---");
  printf("+\n");
}

static void print_board(int size, int board[size]) {
  for (int i = 0; i < size; ++i) {
    print_line_sep(size);
    for (int j = 0; j < size; ++j)
      printf("|%s", board[i] == j ? " Q " : "   ");
    printf("|\n");
  }
  print_line_sep(size);
  printf("\n");
}

int main(int argc, char **argv) {
  if (argc != 2)
    app_error("Usage: %s [SIZE]", argv[0]);

  int size = atoi(argv[1]);

  if (size < 3 || size > 9)
    app_error("Give board size in range from 4 to 9!");

  int board[size];

  /* TODO: A loop is missing here that initializes recursive algorithm. */
	for (int col = 0; col < size; col++) {
		int row = ndselect(size);

		for (int prv = 0; prv < col; prv++) {
			if (conflict(prv, board[prv], col, row)) {
				exit(0);
			}
		}

		board[col] = row;
	}

  print_board(size, board);

  return 0;
}
