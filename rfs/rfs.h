/*
 * rfs.h
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#ifndef RFS_RFS_H_
#define RFS_RFS_H_

#define BLK_SIZE 1 << 12
#define SUPER_MAGIC 0xf0f03410
#define POINTERS_PER_INODE 1 << 2
#define NUM_INDIRECT_POINTERS 1 << 9
#define INODES_PER_BLOCK 1 << 7

struct rfs_superblock{
    int magic_num; //verify integrity of fs
    int num_blocks; // number of blocks in the fs 
    int num_inodeblocks; //number of inode blocks 
    int num_inodes; //num of inodes
};

struct rfs_inode{
    int isvalid; //is in use by a file
    int size; //size of the file in bytes
    int direct[POINTERS_PER_INODE]; //number of direct pointers per file    
    int indirect; //block number which stores the indirect pointers
};

union rfs_block{
    struct rfs_superblock super;
    struct rfs_inode inode[INODES_PER_BLOCK];
    int pointers[NUM_INDIRECT_POINTERS];
    char data[BLK_SIZE];
};

void rfs_debug(); /* scan a mounted fs and prints info on superblock and each inode */

int rfs_format(); //writes a new file system onto the disk,re writes super block
int rfs_mount(); //checks if disk has filesystem, if present read the super block and build free block bitmap

int rfs_create(); //creates a new inode of zero length
int rfs_delete(int inode_num); /*deletes the data held by an inode and resets for use,updates free block bitmap */

int rfs_getsize(int inode_num); //returns the logical size of the inode in bytes

int rfs_read(int inode_num,char *data,int length,int offset); //read data of 'length' bytes from an offset from an inode
int rfs_write(int inode_num,char *data,int length,int offset); //write data of 'length' bytes from an offset


#endif /* RFS_RFS_H_ */
