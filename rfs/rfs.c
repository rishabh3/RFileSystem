/*
 * rfs.c
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#include <string.h>
#include <rfs.h>
#include <../memdisk/memdisk.h>

// Utility function to convert union to string.
void convert_union_to_string(union rfs_block *block_data, char* data){
	memcpy(data, block_data, sizeof(union rfs_block));
}

// Utility function to covert string to union.
void convert_string_to_union(char* data, union rfs_block *block_data){
	memcpy(block_data, data, sizeof(union rfs_block));
}

void print_info(union rfs_block block, int* num_inodes){
	if(num_inodes > )
}

/* scan a mounted fs and prints info on superblock and each inode */
void rfs_debug(){
	union rfs_block s_block, temp;
	char s_block_data_frm_disk[BLK_SIZE];
	disk_read(SUPER_BLOCK, s_block_data_frm_disk);
	convert_string_to_union(s_block_data_frm_disk, &s_block);
	printf("SuperBlock: \n");
	printf("	%d blocks\n", s_block.super.num_blocks);
	printf(" 	%d inode blocks\n", s_block.super.num_inodeblocks);
	printf(" 	%d inodes \n", s_block.super.num_inodes);
	memset(s_block_data_frm_disk, '', BLK_SIZE);
	int first_inode_block = 1;
	for(int i = first_inode_block;i <= s_block.super.num_inodeblocks;i++){
		disk_read(i, s_block_data_frm_disk);
		convert_string_to_union(s_block_data_frm_disk, &temp);
		print_info(temp, &s_block.super.num_inodes);
	}
}

//writes a new file system onto the disk,re writes super block
int rfs_format(){
	return 0;
}

//checks if disk has filesystem, if present read the super block and build free block bitmap
int rfs_mount(){
	return 0;
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



