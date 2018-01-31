/*
 * memdisk.h
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#ifndef MEMDISK_MEMDISK_H_
#define MEMDISK_MEMDISK_H_

#include "../util.h"

#define DISK_BLK_SIZE 4096 // Disk Block Size
#define NUM_BLOCKS 100// Total Disk Size
#define NUM_INODE_BLOCKS (0.1*(NUM_BLOCKS))
#define IN_USE 0x01	// Flags for checking in_use or not
#define READ 1 // Flags for read
#define WRITE 2 // Flags for write
#define FREE 0x00


struct memdisk;

struct memdisk{
	char disk_data[DISK_BLK_SIZE - sizeof(int)];
	int in_use;
};
// This is the in memory disk for storage emulator.

struct memdisk *disk;
bool disk_mounted;
bool new_disk;

void disk_init();
int get_next_free_disk_block_num();
void disk_mount();
void disk_unmount();
void delete_disk();
int disk_size(); // Return the remaining size of the disk.
void disk_read(int ,char *); // Read from the disk.
int disk_write(int ,char *); // Write to the disk.
void disk_unmount();
int reset_stats();


#endif /* MEMDISK_MEMDISK_H_ */
