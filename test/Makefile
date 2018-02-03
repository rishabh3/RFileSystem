CC = gcc
CFLAGS = -g -Wall
OBJDIR = ../bin

all: test

$(OBJDIR)/test.o: test.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/memdisk.o:
	$(MAKE) -C ../memdisk/

$(OBJDIR)/rfs.o:
	$(MAKE) -C ../rfs/

$(OBJDIR)/vrfs.o:
	$(MAKE) -C ../vrfs/

test: $(OBJDIR)/memdisk.o $(OBJDIR)/rfs.o $(OBJDIR)/vrfs.o  $(OBJDIR)/test.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) rshell $(OBJDIR)/*.o *~ ../memdisk/*.gch ../rfs/*.gch ../vrfs/*.gch