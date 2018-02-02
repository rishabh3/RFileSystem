CC = gcc
CFLAGS = -g -Wall
OBJDIR = ../bin

all: $(OBJDIR)/memdisk.o

$(OBJDIR)/memdisk.o: *.c *.h
	$(CC) $(CFLAGS) -c -o $@ $<