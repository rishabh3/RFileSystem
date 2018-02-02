CC = gcc
CFLAGS = -g -Wall

all: rshell

rshell: memdisk.o rfs.o commands.o vrfs.o shell.o
	$(CC) $(CFLAGS) -o $@ $^

memdisk.o: ../memdisk/memdisk.c ../memdisk/memdisk.h
	$(CC) $(CFLAGS) -c $^

rfs.o: ../rfs/rfs.c ../rfs/rfs.h
	$(CC) $(CFLAGS) -c $^

commands.o: commands/commands.c commands/commands.h
	$(CC) $(CFLAGS) -c $^

vrfs.o: ../vrfs/vrfs.c ../vrfs/vrfs.h
	$(CC) $(CFLAGS) -c $^

shell.o: shell.c
	$(CC) $(CLFAGS) -c $^
clean:
	$(RM) rshell *.o *~ *.gch