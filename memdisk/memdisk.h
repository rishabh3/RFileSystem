/*
 * memdisk.h
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#ifndef MEMDISK_MEMDISK_H_
#define MEMDISK_MEMDISK_H_
#include "macros.h"
#include "../rfs/rfs.h"

struct memdisk;

extern struct memdisk{
	union block block_type;
	int in_use;
}memdisk[NUM_BLOCKS];

int get_next_free_inode();
int disk_size();
void disk_read(int ,union block *);
void disk_write(int ,union block *, int);


#endif /* MEMDISK_MEMDISK_H_ */
