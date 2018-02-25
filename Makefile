CC = gcc
CFLAGS = -g -Wall
OBJDIR = bin


all: rshell

$(OBJDIR)/disk.o:
	$(MAKE) -C disk/

$(OBJDIR)/rfsdisk.o:
	$(MAKE) -C rfsdisk/

$(OBJDIR)/vrfs.o:
	$(MAKE) -C vrfs/

$(OBJDIR)/commands.o:
	$(MAKE) -C shell/commands/

$(OBJDIR)/diskshell.o:
	$(MAKE) -C shell/

rshell: $(OBJDIR)/disk.o $(OBJDIR)/rfsdisk.o $(OBJDIR)/vrfs.o $(OBJDIR)/diskshell.o $(OBJDIR)/commands.o 
	touch diskfile
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM)  rshell diskfile $(OBJDIR)/*.o *~ memdisk/*.gch disk/*.gch rfs/*.gch rfsdisk/*.gch vrfs/*.gch shell/*.gch shell/commands/*.gch