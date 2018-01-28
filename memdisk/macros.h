/*
 * macros.h
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#ifndef MEMDISK_MACROS_H_
#define MEMDISK_MACROS_H_

#define DISK_BLK_SIZE 4096
#define DISK_MAGIC 0xafedcbaa
#define NUM_BLOCKS 125000
#define IN_USE 0x01
#define FULL 0x02
#define READ 1
#define WRITE 2
#define SUPERBLOCK 0x0a
#define INODE	0x0b
#define DATA 	0x0c

#endif /* MEMDISK_MACROS_H_ */
