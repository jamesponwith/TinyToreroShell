/*
 * Header file for circular queue of HistoryListItems.
 */
#ifndef __HISTQUEUE_H__
#define __HISTQUEUE_H__

#include "parse_args.h"

#define MAXHIST 10   // max number of commands in history list

/*
 * A struct to keep information one 
 * command in the history of command 
 * executed
 */
typedef struct {              
    unsigned int cmd_num;
    char cmdline[MAXLINE]; // command line for this process
} HistoryEntry;

int histSize();
void addEntry(char new_cmd[MAXLINE]);
void printHistory();
int numToCmd(char *cmd);

#endif
