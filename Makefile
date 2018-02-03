CC = gcc
CFLAGS = -g -Wall
OBJDIR = bin


all: rshell

$(OBJDIR)/memdisk.o:
	$(MAKE) -C memdisk/

$(OBJDIR)/rfs.o:
	$(MAKE) -C rfs/

$(OBJDIR)/vrfs.o:
	$(MAKE) -C vrfs/

$(OBJDIR)/commands.o:
	$(MAKE) -C shell/commands/

$(OBJDIR)/shell.o:
	$(MAKE) -C shell/

rshell: $(OBJDIR)/memdisk.o $(OBJDIR)/rfs.o $(OBJDIR)/vrfs.o $(OBJDIR)/shell.o $(OBJDIR)/commands.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) rshell $(OBJDIR)/*.o *~ memdisk/*.gch rfs/*.gch vrfs/*.gch shell/*.gch shell/commands/*.gch