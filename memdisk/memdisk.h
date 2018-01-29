/*
 * memdisk.h
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#ifndef MEMDISK_MEMDISK_H_
#define MEMDISK_MEMDISK_H_

#define DISK_BLK_SIZE 4096 // Disk Block Size
#define NUM_BLOCKS 125000 // Total Disk Size around 500 MB
#define IN_USE 0x01	// Flags for checking in_use or not
#define READ 1 // Flags for read
#define WRITE 2 // Flags for write


struct memdisk;

struct memdisk{
	char disk_data[DISK_BLK_SIZE - sizeof(int)];
	int in_use;
};
// This is the in memory disk for storage emulator.

struct memdisk *disk;

struct memdisk* disk_init();
int disk_size(); // Return the remaining size of the disk.
void disk_read(int ,char *); // Read from the disk.
void disk_write(int ,char *); // Write to the disk.
void disk_unmount();
int reset_stats();


#endif /* MEMDISK_MEMDISK_H_ */
