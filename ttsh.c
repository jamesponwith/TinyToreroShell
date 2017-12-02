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
#include <sys/types.h> 
#include <sys/wait.h>
#include <sys/errno.h>

#include "parse_args.h"
#include "history_queue.h"

// Forward function declarations 
void execCmd(char *argv[], int ret); 
void cd(char *argv[]); 
void unix_error(char*msg);
pid_t Fork(void); 
void child_handler(__attribute__ ((unused)) int sig);
int builtIn(char *argv[]); 

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

		//fprintf(stdout, "DEBUG: %s\n", cmdline);

		char *argv[MAXARGS];
		int ret = parseArguments(cmdline, argv);

		if (argv[0] == NULL) {
			continue;
		}

		addEntry(cmdline);	

		if (builtIn(argv) == 1) {
			continue;
		}
		else {
			execCmd(argv, ret);
		}
	}
	return 0;
}

int builtIn(char *argv[]) {
	if (strcmp(argv[0], "exit") == 0) {
		fprintf(stdout, "adios...\n");
		exit(0);
	}
	else if (strcmp(argv[0], "history") == 0) {
		printHistory();
		return 1;
	}
	else {
		return 0;
	}
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
	else { // Parent runs this
		if (ret == 0) { //  foregrund
			fprintf(stdout, "\t\t foreground\n");
			waitpid(child_pid, &status, 0); // wait for child
			return;
		}
		else { //background
			fprintf(stdout, "\t\t background\n");
			return; //dont wait for child.
		}
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
	if ((pid = fork()) < 0)
		unix_error("Fork error");
	return pid;
}

/*
 */
void child_handler(__attribute__ ((unused)) int sig) {
	pid_t pid;
	int status;
	while((pid = waitpid(pid, &status, WNOHANG)) != -1) {
		// chill here
	}
}
