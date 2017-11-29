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
static int front = 0;
static int rear = 0;
static int cmd_count = 0;

// TODO: implement your history queue functions here

void addEntry(char new_cmd[MAXLINE]) {
	if (rear < MAXHIST - 1) {
		rear++;
		strcpy(history[rear].cmdline, new_cmd);
		history[rear].cmd_num = cmd_count;
	}
	else if (rear == MAXHIST - 1) {
		rear = 1;
		front++;
		strcpy(history[rear].cmdline, new_cmd);
		history[rear].cmd_num = cmd_count;
	}
}
