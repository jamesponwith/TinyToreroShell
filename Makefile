CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -std=c11
TARGETS = ttsh siesta

TTSH_SRC = ttsh.c history_queue.c

all: $(TARGETS)

ttsh: $(TTSH_SRC) history_queue.h parse_args.o
	$(CC) $(CFLAGS) -o $@ $(TTSH_SRC) parse_args.o

siesta: siesta.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(TARGETS)
