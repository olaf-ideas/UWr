#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>

sigset_t mask;
int sig;
int main() {
	struct winsize ws;
	sigemptyset(&mask);
	sigaddset(&mask, SIGWINCH);
	sigprocmask(SIG_BLOCK, &mask, NULL);
	while (1) {
		sigwait(&mask, &sig);
		ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
		printf("row = %d, col = %d\n", ws.ws_row, ws.ws_col);
	}

	return 0;
}
