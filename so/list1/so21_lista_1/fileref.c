#include "csapp.h"

static char buf[256];

#define LINE1 49
#define LINE2 33
#define LINE3 78

off_t cursor_position(int fd) {
	off_t off = Lseek(fd, 0, SEEK_CUR);
	return off;
}

static void do_read(int fd) { 
	/* TODO: Spawn a child. Read from the file descriptor in both parent and
   * child. Check how file cursor value has changed in both processes. */
	int pid = Fork();
	if (pid) { // parent
		Read(fd, buf, LINE1);
		printf("parent: %d off: %ld -> %s\n", getpid(), cursor_position(fd), buf);
		fflush(stdout);
		Read(fd, buf, LINE2);
		printf("parent: %d off: %ld -> %s\n", getpid(), cursor_position(fd), buf);
		fflush(stdout);
		Read(fd, buf, LINE3);
		printf("parent: %d off: %ld -> %s\n", getpid(), cursor_position(fd), buf);
		fflush(stdout);
		exit(0);
	}
		
	Read(fd, buf, LINE1);
	printf("child: %d off: %ld -> %s\n", getpid(), cursor_position(fd), buf);
	fflush(stdout);
	Read(fd, buf, LINE2);
	printf("child: %d off: %ld -> %s\n", getpid(), cursor_position(fd), buf);
	fflush(stdout);
	Read(fd, buf, LINE3);
	printf("child: %d off: %ld -> %s\n", getpid(), cursor_position(fd), buf);
	fflush(stdout);


  exit(0);
}

static void do_close(int fd) {
  int pid = Fork();

	if (pid) { // child
		sleep(1);
		Read(fd, buf, LINE1);
		printf("child: %d off: %ld -> %s\n", getpid(), cursor_position(fd), buf);
		exit(0);
	}
	
	Close(fd);
	printf("parent: %d off: %ld -> %s\n", getpid(), cursor_position(fd), buf);

	// Wait(pid, NULL, 0);

	// file description does not disappear from the child, only from the parent
	/* TODO: In the child close file descriptor, in the parent wait for child to
   * die and check if the file descriptor is still accessible. */

  exit(0);
}

int main(int argc, char **argv) {
  if (argc != 2)
    app_error("Usage: %s [read|close]", argv[0]);

  int fd = Open("test.txt", O_RDONLY, 0);

  if (!strcmp(argv[1], "read"))
    do_read(fd);
  if (!strcmp(argv[1], "close"))
    do_close(fd);
  app_error("Unknown variant '%s'", argv[1]);
}
