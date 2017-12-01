/*
 * The Tiny Torero Shell (TTSH)
 *
 * Hamez Rodriguez y Smeagol
 *
 */

// NOTE: all new includes should go after the following #define
#define _XOPEN_SOURCE 600

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <sys/errno.h>

#include "parse_args.h"
#include "history_queue.h"


// TODO: add your function prototypes here as necessary
void execCmd(char *argv[], int ret); 
void cd(char *argv[]); 
void unix_error(char*msg);
pid_t Fork(void); 
void child_handler(__attribute__ ((unused)) int sig);

int main() { 
	// TODO: Add a call to sigaction to register your SIGCHLD signal handler
	// here. See the write-up for more details on sigaction.
	struct sigaction sa;
	sa.sa_handler = child_handler;
	sa.sa_flags = SA_NOCLDSTOP;
	sigaction(SIGCHLD, &sa, NULL);

	while(1) {
		// (1) print the shell prompt
		fprintf(stdout, "ttsh> ");  
		fflush(stdout);

		// (2) read in the next command entered by the user
		char cmdline[MAXLINE];
		if ((fgets(cmdline, MAXLINE, stdin) == NULL)
				&& ferror(stdin)) {
			// fgets could be interrupted by a signal.
			// This checks to see if that happened, in which case we simply
			// clear out the error and restart our loop so it re-prompts the
			// user for a command.
			clearerr(stdin);
			continue;
		}

		/*
		 * Check to see if we reached the end "file" for stdin, exiting the
		 * program if that is the case. Note that you can simulate EOF by
		 * typing CTRL-D when it prompts you for input.
		 */
		if (feof(stdin)) {
			fflush(stdout);
			exit(0);
		}

		// TODO: remove this line after you are done testing/debugging.
		//fprintf(stdout, "DEBUG: %s\n", cmdline);

		// TODO: complete top-level steps
		// (3) make a call to parseArguments function to parse it into its argv
		// format
		char *argv[MAXARGS];
		int ret = parseArguments(cmdline, argv);

		if (argv[0] == NULL) {
			continue;
		}

		addEntry(cmdline);	
		// (4) Call a function that will determine how to execute the command
		// that the user entered, and then execute it
		execCmd(argv, ret);
		//historyCmd(argv);
	}
	return 0;
}

/**
 * Execute commands in argv
 */
void execCmd(char *argv[], int ret) {
	if (strcmp(argv[0], "exit") == 0) {
		fprintf(stdout, "adios...\n");
		exit(0);
	}
	else if(strcmp(argv[0], "history") == 0) {
		printHistory();
		return;
	}
	else if(ret == 1) {
		fprintf(stdout,"%s\n", "Background");
	}
	else {
		int status;
		pid_t child_pid; 
		printf("%d\n", ret);
		if ((child_pid = Fork()) == 0) { // Child
			if (execvp(argv[0], argv) == -1) {
				fprintf(stdout, "command does not exist\n");
				exit(0);
			}
		}
		else {
			waitpid(-1, &status, 0);
		}
	}
	ret++; // ret returns 0 prior to this and 1 after
}

/**
 * Execute cd command
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

void child_handler(__attribute__ ((unused)) int sig) {
	int saved_errno = errno;
	while(waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}
	errno = saved_errno;	
}
