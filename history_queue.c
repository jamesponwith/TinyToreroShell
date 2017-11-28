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

static int size = 0;
static int front = 0;
static int back = 0;
static int cmd_count = 0;

// TODO: implement your history queue functions here

int histSize() {
	return size;
}

void addEntry(char new_cmd[MAXLINE]) {
	if (size == 0) {
		// add first entry
		memcpy(history[back].cmdline, new_cmd, sizeof(char[MAXLINE]));
		history[back].cmd_num = cmd_count;
		size++;
	}
	else if (size < MAXHIST - 1) {
		back++;
		cmd_count++;
		size++;
		memcpy(history[back].cmdline, new_cmd, sizeof(char[MAXLINE]));
		history[back].cmd_num = cmd_count;
	}
	else if (size == MAXHIST) {

	}
}
