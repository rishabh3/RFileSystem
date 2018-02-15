CC = gcc
CFLAGS = -g -Wall
OBJDIR = ../bin

all: $(OBJDIR)/shell.o $(OBJDIR)/diskshell.o

$(OBJDIR)/shell.o: shell.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/diskshell.o: diskshell.c
	$(CC) $(CFLAGS) -c -o $@ $<