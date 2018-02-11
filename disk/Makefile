CC = gcc
CFLAGS = -g -Wall
OBJDIR = ../bin

all: $(OBJDIR)/disk.o

$(OBJDIR)/disk.o: *.c *.h
	$(CC) $(CFLAGS) -c -o $@ $<