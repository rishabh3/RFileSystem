CC = gcc
CFLAGS = -g -Wall
OBJDIR = ../bin

all: test

$(OBJDIR)/test.o: test.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/disk.o:
	$(MAKE) -C ../disk/

$(OBJDIR)/rfsdisk.o:
	$(MAKE) -C ../rfsdisk/

$(OBJDIR)/vrfs.o:
	$(MAKE) -C ../vrfs/

$(OBJDIR)/commands.o:
	$(MAKE) -C ../shell/commands/

test: $(OBJDIR)/disk.o $(OBJDIR)/rfsdisk.o $(OBJDIR)/vrfs.o  $(OBJDIR)/test.o $(OBJDIR)/commands.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) test $(OBJDIR)/*.o *~ ../disk/*.gch ../rfsdisk/*.gch ../vrfs/*.gch
