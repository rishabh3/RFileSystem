CC = gcc
CFLAGS = -g -Wall
OBJDIR = ../bin

all: $(OBJDIR)/vrfs.o

$(OBJDIR)/vrfs.o: *.c *.h
	$(CC) $(CFLAGS) -c -o $@ $<