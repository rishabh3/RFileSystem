/*
 * memdisk.h
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#ifndef MEMDISK_MEMDISK_H_
#define MEMDISK_MEMDISK_H_

#define DISK_BLK_SIZE 4096
#define NUM_BLOCKS 125000
#define IN_USE 0x01
#define FULL 0x02
#define READ 1
#define WRITE 2

struct memdisk;

extern struct memdisk{
	char disk_data[DISK_BLK_SIZE - sizeof(int)];
	int in_use;
}memdisk[NUM_BLOCKS];


int disk_size();
void disk_read(int ,union rfs_block *);
void disk_write(int ,union rfs_block *, int);


#endif /* MEMDISK_MEMDISK_H_ */
