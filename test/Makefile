CC = gcc
CFLAGS = -g -Wall
OBJDIR = ../bin

all: test

$(OBJDIR)/testmkdir.o: testmkdir.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/memdisk.o:
	$(MAKE) -C ../memdisk/

$(OBJDIR)/rfs.o:
	$(MAKE) -C ../rfs/

$(OBJDIR)/vrfs.o:
	$(MAKE) -C ../vrfs/

$(OBJDIR)/commands.o:
	$(MAKE) -C ../shell/commands/

test: $(OBJDIR)/memdisk.o $(OBJDIR)/rfs.o $(OBJDIR)/vrfs.o  $(OBJDIR)/testmkdir.o $(OBJDIR)/commands.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) rshell $(OBJDIR)/*.o *~ ../memdisk/*.gch ../rfs/*.gch ../vrfs/*.gch