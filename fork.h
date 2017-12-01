#ifndef __FORK_H_
#define __FORK_H_

#include <sys/types.h>
#include <sys/errno.h>
#include <unistd.h>


void unix_error(char *msg) {
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}

pid_t Fork(void) {
	pid_t pid;

	if ((pid = fork()) < 0)
		unix_error("Fork error");
	return pid;
}

#endif
