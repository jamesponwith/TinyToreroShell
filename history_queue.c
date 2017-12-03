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

static HistoryEntry history[MAXHIST]; 

static int front = 0;
static int rear = 0;
static int cmd_count = 0;

/*
 * Enables the '!num' syntax to execute
 * a command by the command number
 */
void numToCmd(char *cmd) {
	unsigned int cmd_index = atoi(cmd);
	for (int i = 0; i < MAXHIST; i++) { // loop through MAXHIST number of times
		if (cmd_index == history[i].cmd_num) {
			strcpy(cmd, history[i].cmdline);
		}
	}
}

/*
 * Copies the users most recent command
 * into the circular queue while 
 * assiging each with a command number
 */
void addEntry(char new_cmd[MAXLINE]) {
	cmd_count++;

	strcpy(history[rear].cmdline, new_cmd);
	history[rear].cmd_num = cmd_count;

	rear = (rear + 1) % MAXHIST;

	if (cmd_count > MAXHIST) {
		front = (front + 1) % MAXHIST;				
	}
}

/*
 * Loops through the history queue printing
 * the most recent 10 commands
 */
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
