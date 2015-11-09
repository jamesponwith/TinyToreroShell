/*
 * The Tiny Torero Shell (TTSH)
 *
 * Add your top-level comments here.
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>

#include "parse_args.h"
#include "history_queue.h"


// TODO: add your function prototypes here as necessary


int main( ){ 

	char cmdline[MAXLINE];
	char *argv[MAXARGS];
	int bg;

	// TODO: add a call to signal to register your signal handler on 
	//       SIGCHLD here

	while(1) {
		// (1) print the shell prompt
		fprintf(stdout, "ttsh> ");  
		fflush(stdout);

		// (2) read in the next command entered by the user
		if ((fgets(cmdline, MAXLINE, stdin) == NULL) 
				&& ferror(stdin)) {
			perror("fgets error");
		}

		if (feof(stdin)) { /* End of file (ctrl-d) */
			fflush(stdout);
			exit(0);
		}

		fprintf(stdout, "DEBUG: %s\n", cmdline);

		// TODO: complete top-level steps
		// (3) make a call to parseArguments function to parse it into its argv
		// format

		// (4) determine how to execute it, and then execute it

	}

	return 0;
}
