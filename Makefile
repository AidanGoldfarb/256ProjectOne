CC = gcc
CFLAGS = -Wall -g

all: proc_parse shell

proc_parse: proc_parse.c
	$(CC) $(CFLAGS) -o proc_parse proc_parse.c $(LIB)
shell: shell.c
	$(CC) $(CFLAGS) -o shell shell.c $(LIB)
