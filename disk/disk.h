/*
 * disk.h
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#ifndef DISK_DISK_H_
#define DISK_DISK_H_

#include "../util.h"
#include <stdio.h>

#define DISK_BLK_SIZE 4096 // Disk Block Size
#define NUM_BLOCKS 100// Total Disk Size
#define NUM_INODE_BLOCKS (0.1*(NUM_BLOCKS))
#define NUM_D_BLOCKS ((NUM_BLOCKS) - (NUM_INODE_BLOCKS))
#define READ 1 // Flags for read
#define WRITE 2 // Flags for write
#define DATA_SIZE  (DISK_BLK_SIZE) // Macro for size of data to write or read from disk.

char file_name[1024];


FILE* disk ; // File pointer to disk file
bool disk_mounted; // Flags to check if disk is mounted.

void disk_init(char *filename, char* mode); // Initialization of disk.
int get_next_free_disk_block_num(); // Returns the next free disk block number.
void disk_mount(); // Mount the disk 
void disk_unmount(); // Unmount the disk
void disk_close(); // Delete the disk free up the disk space.
int disk_size(); // Return the remaining size of the disk.
void disk_read(int ,char *); // Read from the disk.
int disk_write(int ,char *); // Write to the disk.
int reset_stats(); // Reset the status variables.




#endif /* DISK_DISK_H_ */
