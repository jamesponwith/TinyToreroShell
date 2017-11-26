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

#include "parse_args.h"
#include "history_queue.h"


// TODO: add your function prototypes here as necessary
void execCmd(char *argv[]); 


int main() { 
	// TODO: Add a call to sigaction to register your SIGCHLD signal handler
	// here. See the write-up for more details on sigaction.

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
		fprintf(stdout, "DEBUG: %s\n", cmdline);

		// TODO: complete top-level steps
		// (3) make a call to parseArguments function to parse it into its argv
		// format
		char *argv[MAXARGS];
		int ret = parseArguments(cmdline, argv);
		if (ret != 0) {
			printf("something went wrong");
		}

		// (4) Call a function that will determine how to execute the command
		// that the user entered, and then execute it
		execCmd(argv);
	}

	return 0;
}

/**
 * Execute commands in argv
 */
void execCmd(char *argv[]) {
	if (strcmp(argv[0], "exit") == 0) {
		printf("adios...\n");
		exit(0);
	}

}
