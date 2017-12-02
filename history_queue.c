/*
 * The Tiny Torero Shell (TTSH)
 *
 * This class sets up a data structure to
 * represent a circular queue which will 
 * be used to store the history of commands.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "history_queue.h"

// global variables: add only globals for history list state
//                   all other variables should be allocated on the stack
// static: means these variables are only in scope in this .c module

static HistoryEntry history[MAXHIST]; 

// Look at your lab 10 implementation of a circular queue of integers and
// reuse liberally.

// TODO: Add some more global variables for the history list.
//       These should be the only globals in your program
//       They should be static so that only functions in this file can
//       access them.

static int front = 0;
static int rear = 0;
static int cmd_count = 0;

// TODO: implement your history queue functions here
//

void numToCmd(char *cmd) {
	unsigned int cmd_index = atoi(cmd);
	for (int i = 0; i < MAXHIST; i++) { // loop through MAXHIST number of times
		if (cmd_index == history[i].cmd_num) {
			strcpy(cmd, history[i].cmdline);
		}
	}
}

void addEntry(char new_cmd[MAXLINE]) {
	cmd_count++;

	strcpy(history[rear].cmdline, new_cmd);
	history[rear].cmd_num = cmd_count;

	rear = (rear + 1) % MAXHIST;

	if (cmd_count > MAXHIST) {
		front = (front + 1) % MAXHIST;				
	}
}

void printHistory() {
	int j = front;
	for (int i = 0; i < MAXHIST; i++) { // loop through MAXHIST number of times
		if (strcmp("", history[j].cmdline) != 0) {
			fprintf(stdout, "%u\t%s", history[j].cmd_num, history[j].cmdline);
		}
		j++;
		if (j == MAXHIST) {
			j = 0;
		}
	}
}
