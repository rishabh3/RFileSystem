/*
 * rfs.c
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rfs.h"
#include "../memdisk/memdisk.h"

#define is_set(flag)	(flag == 1 ? 1 : 0)
static int num_inode_block = 0; // Current Inode Block in use keep track the inode block in use.
static int num_inode = 0; // Current Inode in use keep track the inode in use and reset it when the inode block becomes full
static int inode_num = 0; // Keep track of new inode num

// Utility function to convert union to string.
void convert_union_to_string(void *block_data, char* data, unsigned long int size){
	memcpy(data, block_data, size);
	data = strcat(data, "\000");
}

void manage_inode_counters(){
	/* Could be implemented better.
	 * Currently, only uses a forward scan. If an inode gets freed up,
	 * we cant use this recently freed inode.
	 */
	if(num_inode == INODES_PER_BLOCK-1){
		num_inode_block++;
		num_inode = 0;
	}
	else{
		num_inode++;
	}
}
// Utility function to covert string to union.
void convert_string_to_union(char* data, void *block_data, unsigned long int size){
	memcpy(block_data, data,  size);
}

// Inode seek from the disk
void seek_inode_block_inode_num(int inode_num, int* disk_block, int* inode_index){
	*disk_block = (int)(inode_num / INODES_PER_BLOCK) + 1;
	*inode_index =  inode_num % INODES_PER_BLOCK - 1;
}


void get_data_frm_disk(int block_num, char * block_data){
	disk_read(block_num, block_data);
}

int generate_new_inode_num(){
	inode_num++;
	return inode_num;
}

int write_data_to_disk(int block_num, void *block, unsigned long int size){
	char block_data[BLK_SIZE];
	convert_union_to_string(block, block_data, size);
	if(!disk_write(block_num, block_data)){
		return 0;
	}
	return 1;
}

void print_info(union rfs_block block, int* num_inodes){
	if(*num_inodes >= INODES_PER_BLOCK){
		*num_inodes -= INODES_PER_BLOCK;
	}
	int prev_i = -1;
	for(int i = 0;i < INODES_PER_BLOCK;i++){
		if(is_set(block.inode[i].isvalid) && prev_i != i){
			prev_i = i;
			printf("Inode %d: \n", i);
			printf(" 	%d inode number \n", block.inode[i].inode_num);
			printf(" 	%d file size \n", block.inode[i].size);
			switch(block.inode[i].type){
				case F_TYPE: printf(" 	Regular File\n");
						break;
				case DIR_TYPE: printf("	Directory \n");
						break;
			}
		}
//		else{
//			printf("DEBUG: NO VALID INODE\n");
//		}
	}
}

/* scan a mounted fs and prints info on superblock and each inode */
void rfs_debug(){
	union rfs_block s_block, temp;
	char data[BLK_SIZE];
	get_data_frm_disk(SUPER_BLOCK, data);
	convert_string_to_union(data, (void*)(&s_block.super), sizeof(struct rfs_superblock));
	//memcpy(&(s_block.super), blk_data, sizeof(struct rfs_superblock));
	printf("SuperBlock: \n");
	printf(" 	%d magic number \n", s_block.super.magic_num);
	printf("	%d blocks\n", s_block.super.num_blocks);
	printf(" 	%d inode blocks\n", s_block.super.num_inodeblocks);
	printf(" 	%d inodes \n", s_block.super.num_inodes);
	int first_inode_block = 1;
	memset(data, ' ', BLK_SIZE);
	for(int i = first_inode_block;i <= num_inode_block+1;i++){
		get_data_frm_disk(i, data);
		convert_string_to_union(data, (void*)(&(temp.inode)), sizeof(struct rfs_inode));
		//memcpy(&(temp.inode) ,blk_data, sizeof(struct rfs_inode)*INODES_PER_BLOCK);
		printf("Block Number: %d \n", i);
		print_info(temp, &s_block.super.num_inodes);
	}
}

int unset_invalid_bit(union rfs_block *block){
	for(int i = 0;i < INODES_PER_BLOCK; i++){
		if(is_set(block->inode[i].isvalid)){
			block->inode[i].isvalid = 0;
		}
	}
	return 1;
}

//writes a new file system onto the disk,re writes super block
int rfs_format(){
	int num_inodeblocks = (int)((10*NUM_BLOCKS)/100); // Keeping only 10% as inodes.
	union rfs_block block, temp;
	if(!disk || disk_mounted){
		return 0;
	}
	else if(new_disk){
		block.super.magic_num = SUPER_MAGIC;
		block.super.num_blocks = NUM_BLOCKS;
		//printf("DEBUG: %d\n", num_inodeblocks);
		block.super.num_inodeblocks = num_inodeblocks;
		//printf("DEBUG: %d\n", block.super.num_inodeblocks);
		block.super.num_inodes = (INODES_PER_BLOCK)*num_inodeblocks;
		//printf("DEBUG: %d\t%d\n", block.super.num_inodes, num_inodeblocks*(INODES_PER_BLOCK));
		if(!write_data_to_disk(SUPER_BLOCK, (void *)&(block.super), sizeof(struct rfs_superblock))){
			return 0;
		}
		for(int i = 1;i <= num_inodeblocks;i++){
			unset_invalid_bit(&(temp));
			if(!write_data_to_disk(i, (void *)&(temp.inode), sizeof(union rfs_block))){
				return 0;
			}
		}
		new_disk = FALSE;
	}
	else{
		//else clear superblock and inodes and reset it. Call reset status
		// Get superblock from disk reset it
		char data[BLK_SIZE];
		get_data_frm_disk(SUPER_BLOCK, data);
		convert_string_to_union(data, (&(block.super)), sizeof(struct rfs_superblock));
		//memcpy(&(block.super), blk_data, sizeof(struct rfs_superblock));
		block.super.num_blocks = NUM_BLOCKS;
		block.super.num_inodeblocks = num_inodeblocks;
		block.super.num_inodes = INODES_PER_BLOCK * num_inodeblocks;
		if(!write_data_to_disk(SUPER_BLOCK, (void *)&(block.super), sizeof(struct rfs_superblock))){
			return 0;
		}
		memset(data, ' ', BLK_SIZE);
		// Get all inodes and then reset the valid bits.
		for(int i = 1;i <= num_inodeblocks;i++){
			get_data_frm_disk(i, data);
			convert_string_to_union(data, (&(temp.inode)), sizeof(struct rfs_inode));
			//memcpy(&(block.inode) ,blk_data, sizeof(struct rfs_inode));
			unset_invalid_bit(&temp);
			if(!write_data_to_disk(i, (void *)&(temp.inode), sizeof(union rfs_block))){
				// This i think needs to be checked properly. proper testing
				return 0;
			}
		}
	}
	// Reset the stats for the disk
	reset_stats();
	// Note : Data is not erased.
	return 1;
}

//checks if disk has filesystem, if present read the super block and build free block bitmap
int rfs_mount(){
	disk_mount();
	if(!disk){
			return 0;
	}
	union rfs_block block;
	char data[BLK_SIZE];
	get_data_frm_disk(SUPER_BLOCK, data);
	convert_string_to_union(data, (void*)(&block.super), sizeof(struct rfs_superblock));
	//memcpy(&(block.super), blk_data, sizeof(struct rfs_superblock));
	bitmap = (char *)malloc(sizeof(char)*NUM_BLOCKS);
	for(int i = 0;i < block.super.num_blocks;i++){
		bitmap[i] = '1';
	}
	return 1;
}

//creates a new inode of zero length
int rfs_create(int size){
	union rfs_block block;
	block.inode[num_inode].isvalid = 1;
	block.inode[num_inode].inode_num = generate_new_inode_num();
	block.inode[num_inode].size = size;
	block.inode[num_inode].type = F_TYPE;
	manage_inode_counters();
	if(!write_data_to_disk(num_inode_block+1, (void *)&(block.inode), sizeof(union rfs_block))){
		return 0;
	}
	return 1;
}

/*deletes the data held by an inode and resets for use,updates free block bitmap */
int rfs_delete(int inode_num){
	return 0;
}

//returns the logical size of the inode in bytes
int rfs_getsize(int inode_num){
	int inode_block_num, inode_index;
	union rfs_block block;
	char data[BLK_SIZE];
	seek_inode_block_inode_num(inode_num, &inode_block_num, &inode_index);
	get_data_frm_disk(inode_block_num, data);
	convert_string_to_union(data, (void*)(&(block.inode)), sizeof(union rfs_block));
	if(is_set(block.inode[inode_index].isvalid)){
		return block.inode[inode_index].size;
	}
	return -1;
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
	free(bitmap);
	bitmap = 0x00;
	delete_disk();
	return 0;
}



