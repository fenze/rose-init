#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdnoreturn.h>

noreturn static inline void die(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(void)
{
	sigset_t set;

	if (getpid() != 1)
		return 1;

	if (sigfillset(&set) == -1)
		die("sigfillset");

	if (sigprocmask(SIG_BLOCK, &set, 0) == -1)
		die("sigprocmask");

	if (fork()) {
		while (1) {
			int status;
			pid_t pid = wait(&status);

			if (pid == -1)
				die("wait");
		}
	}

	if (sigprocmask(SIG_UNBLOCK, &set, NULL) == -1)
		die("sigprocmask");

	if (setsid() == -1)
		die("setsid");

	if (setpgid(0, 0) == -1)
		die("setpgid");

	if (execve("/etc/rc", (char *const[]) { "rc", 0 }, (char *const[]) { 0 }) == -1)
		die("execve");

	return 0;
}
