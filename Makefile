CC = gcc
CFLAGS = -Wall -g

all: proc_parse

proc_parse: proc_parse.c
	$(CC) $(CFLAGS) -o proc_parse proc_parse.c $(LIB)