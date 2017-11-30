/*
 * The Tiny Torero Shell (TTSH)
 *
 * Add your top-level comments here.
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

//static int size = 0;
static int front = -1;
static int rear = -1;
static int cmd_count = 0;

// TODO: implement your history queue functions here

void addEntry(char new_cmd[MAXLINE]) {
	if (( rear == MAXHIST - 1 && front == 0) || (front = rear + 1)) {
		// full
		strcpy(history[rear].cmdline, new_cmd);
		history[rear].cmd_num = cmd_count;
		// increment rear
		if (rear == MAXHIST - 1) {
			rear = 0;
			front++;
		}
		else if (rear == MAXHIST - 2) {
			rear++;
			front = 0;
		}
		else {
			rear++;
			front++;
		}
	}
	else {
		// not full
		strcpy(history[rear + 1].cmdline, new_cmd);
		history[rear + 1].cmd_num = cmd_count;
		if (rear == MAXHIST - 1) {
			rear = 0;
		}
		else {
			rear++;
		}
	}
}
