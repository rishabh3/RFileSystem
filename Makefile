CC = gcc
CFLAGS = -g -Wall
OBJDIR = bin


all: rmemshell rdiskshell

$(OBJDIR)/memdisk.o:
	$(MAKE) -C memdisk/

$(OBJDIR)/disk.o:
	$(MAKE) -C disk/

$(OBJDIR)/rfs.o:
	$(MAKE) -C rfs/

$(OBJDIR)/rfsdisk.o:
	$(MAKE) -C rfsdisk/

$(OBJDIR)/vrfs.o:
	$(MAKE) -C vrfs/

$(OBJDIR)/commands.o:
	$(MAKE) -C shell/commands/

$(OBJDIR)/shell.o:
	$(MAKE) -C shell/

rmemshell: $(OBJDIR)/memdisk.o $(OBJDIR)/rfs.o $(OBJDIR)/vrfs.o $(OBJDIR)/shell.o $(OBJDIR)/commands.o
	$(CC) $(CFLAGS) -o $@ $^

rdiskshell: $(OBJDIR)/disk.o $(OBJDIR)/rfsdisk.o $(OBJDIR)/vrfs.o $(OBJDIR)/shell.o $(OBJDIR)/commands.o 
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) rmemshell rdiskshell $(OBJDIR)/*.o *~ memdisk/*.gch disk/*.gch rfs/*.gch rfsdisk/*.gch vrfs/*.gch shell/*.gch shell/commands/*.gch