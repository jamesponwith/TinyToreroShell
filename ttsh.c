/*
 * The Tiny Torero Shell (TTSH)
 *
 * James Ponwith and Patrick Hall 
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
#include <libgen.h>

#include "parse_args.h"
#include "history_queue.h"

pid_t Fork(void); 
void cd(char *argv[]); 
void unix_error(char*msg);
void nextDir(char *argv[]); 
int isBuiltIn(char *argv[]); 
int isBangNum(char cmd[MAXLINE]);
int shellEntry(char cmdline[MAXLINE]); 
void execCmd(char *argv[], int ret); 
void child_handler(__attribute__ ((unused)) int sig);

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
		if (shellEntry(cmdline) == 1) {
			continue;
		}
	}
	return 0;
}

/* 
 * Takes in a command and parses it, determines
 * the type of command, then exectures the 
 * command
 *
 * @param cmdline[MAXLINE] the users command] 
 * @return 1 if command is to run in background
 * @return 0 if command is to run in foreground
 */
int shellEntry(char cmdline[MAXLINE]) {
	char *argv[MAXARGS];
	int add	 = isBangNum(cmdline);
	if (add == 0) {
		fprintf(stdout, "ERROR, command not in history\n");
		return 0;
	}

	int ret = parseArguments(cmdline, argv);
	if (argv[0] == NULL) {
		return 1; 
	}
	if (add == 1) {
		addEntry(cmdline);	
	}
	// else break
	if (isBuiltIn(argv) == 1) {
		return 1;
	}
	execCmd(argv, ret);
	return 0;
}

/* 
 * Determines if command is built in to ttsh.c
 * If so, this executes the command
 *
 * @param argv Command line arguments
 * @return 0 if not
 * @return 1 if it is 
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
 * Determines if the command is of the format !num
 * @param *cmd argv[1]
 * @return ret 1 if command is in history
 * 			   0 if not
 */
int isBangNum(char *cmd) {
	int ret = 1;
	if (cmd[0] == '!') {
		memmove(cmd, cmd+1, strlen(cmd));
		ret = numToCmd(cmd);
	}
	return ret;
}


/*
 * Functionality for the cd command
 * @param argv Command line arguments
 */
void cd(char *argv[]) {
	if (argv[1] == NULL) {
		chdir(getenv("HOME"));
		return;
	}
	else if (strcmp(argv[1], "..") != 0) {
		chdir(argv[1]);
		//nextDir(argv);
		return;
	}
	else {
		char cwd[MAXLINE];
		getcwd(cwd, sizeof(cwd));
		chdir(dirname(cwd));
		return;
	}
}

/*
 * Changes cwd to the dir specified by argv[1]
 * @param argv Command line arguments
 */
void nextDir(char *argv[]) {
	char *new_cwd = NULL;
	char cwd[MAXLINE];
	getcwd(cwd, sizeof(cwd));

	new_cwd = strcat(cwd, "/");
	new_cwd = strcat(new_cwd, argv[1]);
	if(chdir(new_cwd) == -1) {
		fprintf(stdout, "directory does not exist\n");
	}
	return;
}

/*
 * Execute commands in argv
 * @param argv Command line arguments
 * @param ret Determines if command to run foreground or bakckground
*/
void execCmd(char *argv[], int ret) {
	int status;
	pid_t child_pid; 		
	if ((child_pid = Fork()) == 0) { // Child runs this
		setpgid(0, 0);
		if (execvp(argv[0], argv) == -1) {
			fprintf(stdout, "ERROR: command not found\n");
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
 * Error handling for the fork() method
 */
pid_t Fork(void) {
	pid_t pid;
	if ((pid = fork()) < 0) {
		unix_error("Fork error");
	}
	return pid;
}

/*
 * Handles the child and makes the parent process wait
 * as the child process executes. Additionally, waitpid
 * reaps all children. 
 *
 * @param int sig Attribute unused 
 */
void child_handler(__attribute__ ((unused)) int sig) {
	pid_t pid;
	int status;
	while((pid = waitpid(pid, &status, WNOHANG)) != -1) {}
}
