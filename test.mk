CFLAGS = -g -Wall
CC = gcc

all: rfstest

rfstest: test.o memdisk.o rfs.o
	$(CC) $(CFLAGS) -o rfstest test.o memdisk.o rfs.o

test.o: 
	$(CC) $(CFLAGS) -c test.c memdisk/memdisk.h rfs/rfs.h

memdisk.o:
	$(CC) $(CFLAGS) -c memdisk/memdisk.c memdisk/memdisk.h

rfs.o: 
	$(CC) $(CFLAGS) -c rfs/rfs.c rfs/rfs.h

rm:
	$(RM) rfstest *.o *~ *.gch