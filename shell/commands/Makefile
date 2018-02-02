CC = gcc
CFLAGS = -g -Wall
OBJDIR = ../../bin

all: $(OBJDIR)/commands.o

$(OBJDIR)/commands.o: *.c *.h
	$(CC) $(CFLAGS) -c -o $@ $<

