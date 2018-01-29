/*
 * rfs.c
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#include <string.h>
#include "rfs.h"
#include "../memdisk/memdisk.h"

#define is_set(flag)	(flag == 1 ? 1 : 0)
static int num_inode_block = 0; // Current Inode Block in use keep track the inode block in use.
static int num_inode = 0; // Current Inode in use keep track the inode in use and reset it when the inode block becomes full
static int inode_num = 0; // Keep track of new inode num

union rfs_block get_data_frm_disk(int block_num){
	char block_data[BLK_SIZE];
	union rfs_block kblk;
	disk_read(block_num, block_data);
	convert_string_to_union(block_data, &kblk);
	return kblk;
}

int generate_new_inode_num(){
	inode_num++;
	return inode_num;
}

int write_data_to_disk(int block_num, union rfs_block *block){
	char block_data[BLK_SIZE];
	convert_union_to_string(block, block_data);
	disk_write(block_num, block_data);
	return 1;
}
// Utility function to convert union to string.
void convert_union_to_string(union rfs_block *block_data, char* data){
	memcpy(data, block_data, sizeof(union rfs_block));
}

// Utility function to covert string to union.
void convert_string_to_union(char* data, union rfs_block *block_data){
	memcpy(block_data, data, sizeof(union rfs_block));
}

void print_info(union rfs_block block, int* num_inodes){
	if(*num_inodes >= INODES_PER_BLOCK){
		*num_inodes -= INODES_PER_BLOCK;
	}
	for(int i = 0;i < INODES_PER_BLOCK;i++){
		if(block.inode[i].isvalid){
			printf("Inode %d: \n", i);
			printf(" 	%d file size \n", block.inode[i].size);
			switch(block.inode[i].type){
				case 1: printf(" 	Regular File\n");
						break;
				case 2: printf("	Directory \n");
						break;
			}
		}
	}
}

/* scan a mounted fs and prints info on superblock and each inode */
void rfs_debug(){
	union rfs_block s_block, temp;
	s_block = get_data_frm_disk(SUPER_BLOCK);
	printf("SuperBlock: \n");
	printf("	%d blocks\n", s_block.super.num_blocks);
	printf(" 	%d inode blocks\n", s_block.super.num_inodeblocks);
	printf(" 	%d inodes \n", s_block.super.num_inodes);
	int first_inode_block = 1;
	for(int i = first_inode_block;i <= s_block.super.num_inodeblocks;i++){
		temp = get_data_frm_disk(i);
		print_info(temp, &s_block.super.num_inodes);
	}
}

int unset_invalid_bit(union rfs_block *block){
	if(is_set(block->inode.isvalid)){
		block->inode.isvalid = 0;
	}
	return 1;
}

//writes a new file system onto the disk,re writes super block
int rfs_format(){
	int num_inodeblocks = (int)(10/100*NUM_BLOCKS); // Keeping only 10% as inodes.
	if(!disk){
		return 0;
	}
	//else clear superblock and inodes and reset it. Call reset status
	union rfs_block block;
	// Get superblock from disk reset it
	block = get_data_frm_disk(SUPER_BLOCK);
	block.super.num_blocks = NUM_BLOCKS;
	block.super.num_inodeblocks = num_inodeblocks;
	block.super.num_inodes = INODES_PER_BLOCK * num_inodeblocks;
	write_data_to_disk(SUPER_BLOCK, &block);
	// Get all inodes and then reset the valid bits.
	for(int i = 1;i <= num_inodeblocks;i++){
		block = get_data_from_disk(i);
		unset_invalid_bit(&block);
		write_data_to_disk(i, &block);
	}
	// Reset the stats for the disk
	reset_stats();
	// Note : Data is not erased.
	return 1;
}

//checks if disk has filesystem, if present read the super block and build free block bitmap
int rfs_mount(){
	disk = disk_init();
	if(!disk){
		return 0;
	}
	return 1;
}

//creates a new inode of zero length
int rfs_create(){
	return 0;
}

/*deletes the data held by an inode and resets for use,updates free block bitmap */
int rfs_delete(int inode_num){
	return 0;
}

//returns the logical size of the inode in bytes
int rfs_getsize(int inode_num){
	return 0;
}

//read data of 'length' bytes from an offset from an inode
int rfs_read(int inode_num,char *data,int length,int offset){
	return 0;
}

//write data of 'length' bytes from an offset
int rfs_write(int inode_num,char *data,int length,int offset){
	return 0;
}

int rfs_unmount(){
	disk_unmount();
	return 0;
}



