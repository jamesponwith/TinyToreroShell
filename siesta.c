/*  
 * A long running program for testing the Tiny Torero shell program.
 * It can run with or without a command line argument that specifies
 * how many times the program should sleep for 2 seconds before 
 * exiting.
 * With no command line argument it does this 5 times (i.e. 10 times).
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
	long num = 5;

	if(argc == 2) {
		num = strtol(argv[1], NULL, 10);
	}
	else if (argc > 2) {
		fprintf(stderr, "usage: %s [number of 2 second intervals to sleep]\n", argv[0]);
		exit(1);
	}

	pid_t my_pid = getpid();

	printf("pid = %d is going to have a siesta!\n", my_pid);
	for(int i = 0; i < num; i++) {
		sleep(2);     
	}
	printf("pid = %d woke up. Time for a fiesta!\n", my_pid);

	return 0;
}
