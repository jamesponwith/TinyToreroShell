/*
 * The Tiny Torero Shell (TTSH)
 *
 * Hamez Rodriguez y Smeagol
 *
 * This program implements a linux shell. Capable of executing 
 * commands both, in foreground and background. It uses 
 * commands such as fork, execvp, and waitpid system calls to
 * control processes. 
 */

#define _XOPEN_SOURCE 600

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <sys/errno.h>
#include <assert.h>

#include "parse_args.h"
#include "history_queue.h"

// Forward function declarations 
void execCmd(char *argv[], int ret); 
void cd(char *argv[]); 
void unix_error(char*msg);
pid_t Fork(void); 
void child_handler(__attribute__ ((unused)) int sig);
int isBuiltIn(char *argv[]); 
void isBangNum(char cmd[MAXLINE]);

int main() { 
	struct sigaction sa;
	sa.sa_handler = child_handler;
	sa.sa_flags = SA_NOCLDSTOP;
	sigaction(SIGCHLD, &sa, NULL);

	while(1) {
		fprintf(stdout, "ttsh> ");  
		fflush(stdout);

		char cmdline[MAXLINE];
		if ((fgets(cmdline, MAXLINE, stdin) == NULL)
				&& ferror(stdin)) {
			clearerr(stdin);
			continue;
		}

		if (feof(stdin)) {
			fflush(stdout);
			exit(0);
		}

		char *argv[MAXARGS];

		isBangNum(cmdline);

		int ret = parseArguments(cmdline, argv);
		if (argv[0] == NULL) {
			continue;
		}

		addEntry(cmdline);	
		if (isBuiltIn(argv) == 1) {
			continue;
		}

		execCmd(argv, ret);
	}
	return 0;
}

void isBangNum(char *cmd) {
	if (cmd[0] == '!') {
		memmove(cmd, cmd+1, strlen(cmd));
		numToCmd(cmd);
	}
}


/**
 * Execute cd command
 * @param *argv[] The vector of of current command line 
 * arguements to navigate the system using cd command 
 * syntax 
 */
void cd(char *argv[]) {
	char cmdline[MAXLINE];
  	//getenv(cmdline);
	if (argv[1] == NULL) {
		chdir(getenv(cmdline));
		return;
	}
	else if (strcmp(argv[1], "..")) {
		chdir(cmdline);
		return;
	}
}

/*
 * Checks if command is built in this program
 * executes command and returns 1 if so
 * returns 0 if not
 */
int isBuiltIn(char *argv[]) {
	if (strcmp(argv[0], "exit") == 0) {
		fprintf(stdout, "adios...\n");
		exit(0);
	}
	else if (strcmp(argv[0], "history") == 0) {
		printHistory();
		return 1;
	}
	else if (strcmp(argv[0], "cd") == 0) {
		cd(argv);
		return 1;
	}
	return 0;
}

/*
 * Execute commands in argv
*/

void execCmd(char *argv[], int ret) {
	int status;
	pid_t child_pid; 		
	if ((child_pid = Fork()) == 0) { // Child runs this
		setpgid(0, 0);
		if (execvp(argv[0], argv) == -1) {
			fprintf(stdout, "command no es aqui\n");
			exit(0);
		}
	}
	else if (ret == 0) { //  foregrund
		waitpid(child_pid, &status, 0); // wait for child
	}	
}


/*
 * Prints out a message is an error has occured
 * @param *msg A message to be displayed to address
 * the error that has occured
 */
void unix_error(char *msg) {
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}

/*
 */
pid_t Fork(void) {
	pid_t pid;
	if ((pid = fork()) < 0) {
		unix_error("Fork error");
	}
	return pid;
}

/*
 */
void child_handler(__attribute__ ((unused)) int sig) {
	pid_t pid;
	int status;
	while((pid = waitpid(pid, &status, WNOHANG)) != -1) {}
}
