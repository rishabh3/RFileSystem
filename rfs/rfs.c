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
#include <time.h>
#include "../memdisk/memdisk.h"

#define is_set(flag)	(flag == 1 ? 1 : 0) // Macro to check if flag is set.
static int num_inode_block = 0; // Current Inode Block in use keep track the inode block in use.
static int num_inode = 0; // Current Inode in use keep track the inode in use and reset it when the inode block becomes full
static int inode_num = 0; // Keep track of new inode num


void convert_union_to_string(void *block_data, char* data, unsigned long int size){
	
	/*
		Utility function to convert union to string.
	*/

	memcpy(data, block_data, size);
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

void convert_string_to_union(char* data, void *block_data, unsigned long int size){
	
	/*
		Utility function to convert union to string.
	*/
	
	memcpy(block_data, data,  size);
}

void seek_inode_block_inode_num(int inode_num, int* disk_block, int* inode_index){

	/*
		Utility function to seek the block number and inode index given the inode number. 
	*/

	*disk_block = (int)(inode_num / INODES_PER_BLOCK) + 1;
	*inode_index =  inode_num % INODES_PER_BLOCK - 1;
}



void get_data_frm_disk(int block_num, char * block_data){

	/* 
		Utility function to get the data from disk at given block number.
	*/

	disk_read(block_num, block_data);
}

/* struct rfs_inode read_data_frm_disk(int block_num){
	char data[BLK_SIZE];
	get_data_frm_disk(block_num, data);
	union rfs_block block;
	convert_string_to_union(data,(void *)(&(block.inode)), sizeof(union rfs_block));
	return block.inode[0];
} */ 

int generate_new_inode_num(){

	/*
		Generate a new Inode number.
	*/

	inode_num++;
	return inode_num;
}

int write_data_to_disk(int block_num, void *block, unsigned long int size){

	/* 
		Write Data to Disk functionality which maps all 
		in memory structures to string data in disk.
	*/

	char block_data[BLK_SIZE];
	convert_union_to_string(block, block_data, size);
	if(!disk_write(block_num, block_data)){
		return 0;
	}
	return 1;
}

void print_info(union rfs_block block, int* num_inodes){

	/*
		Print the information of inodes.
	*/ 

	if(*num_inodes >= INODES_PER_BLOCK){
		*num_inodes -= INODES_PER_BLOCK;
	}
	for(int i = 0;i < INODES_PER_BLOCK;i++){
		if(is_set(block.inode[i].isvalid)){
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
	}
}


void rfs_debug(){
	
	/* 
		scan a mounted fs and prints info on superblock and each inode 
	*/

	union rfs_block s_block, temp;
	char data[BLK_SIZE];
	get_data_frm_disk(SUPER_BLOCK, data);
	convert_string_to_union(data, (void*)(&(s_block.super)), sizeof(union rfs_block));
	printf("SuperBlock: \n");
	printf(" 	%d magic number \n", s_block.super.magic_num);
	printf("	%d blocks\n", s_block.super.num_blocks);
	printf(" 	%d inode blocks\n", s_block.super.num_inodeblocks);
	printf(" 	%d inodes \n", s_block.super.num_inodes);
	int first_inode_block = 1;
	memset(data, ' ', BLK_SIZE);
	for(int i = first_inode_block;i <= num_inode_block+1;i++){
		get_data_frm_disk(i, data);
		convert_string_to_union(data, (void*)(&(temp.inode)), sizeof(union rfs_block));
		print_info(temp, &s_block.super.num_inodes);
	}
}

int unset_invalid_bit(union rfs_block *block){

	/*
		Unset Invalid Bit for the inodes. 
		To make it available for use next time.
	*/

	for(int i = 0;i < INODES_PER_BLOCK; i++){
		if(is_set(block->inode[i].isvalid)){
			block->inode[i].isvalid = 0;
		}
	}
	return 1;
}

int rfs_format(){

	/*
		Writes the Super Block and create all the inodes.
	*/

	int num_inodeblocks = (int)((10*NUM_BLOCKS)/100); // Keeping only 10% as inodes.
	union rfs_block block, temp;
	if(!disk || disk_mounted){
		return 0;
	}
	else if(new_disk){
		block.super.magic_num = SUPER_MAGIC;
		block.super.num_blocks = NUM_BLOCKS;
		block.super.num_inodeblocks = num_inodeblocks;
		block.super.num_inodes = (INODES_PER_BLOCK)*num_inodeblocks;
		if(!write_data_to_disk(SUPER_BLOCK, (void *)(&(block.super)), sizeof(struct rfs_superblock))){
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
		char data[BLK_SIZE];
		get_data_frm_disk(SUPER_BLOCK, data);
		convert_string_to_union(data, (&(block.super)), sizeof(struct rfs_superblock));
		block.super.num_blocks = NUM_BLOCKS;
		block.super.num_inodeblocks = num_inodeblocks;
		block.super.num_inodes = INODES_PER_BLOCK * num_inodeblocks;
		if(!write_data_to_disk(SUPER_BLOCK, (void *)&(block.super), sizeof(struct rfs_superblock))){
			return 0;
		}
		memset(data, ' ', BLK_SIZE);
		for(int i = 1;i <= num_inodeblocks;i++){
			get_data_frm_disk(i, data);
			convert_string_to_union(data, (&(temp)), sizeof(struct rfs_inode));
			unset_invalid_bit(&temp);
			if(!write_data_to_disk(i, (void *)&(temp.inode), sizeof(union rfs_block))){
				return 0;
			}
		}
	}
	reset_stats();
	// Note : Data is not erased.
	return 1;
}

int rfs_mount(){

	/*
		checks if disk has filesystem, if present read the super block and build free block bitmap
	*/

	if(new_disk || disk_mounted){
		return 0;
	}
	disk_mount();
	if(!disk){
			return 0;
	}
	union rfs_block block;
	char data[BLK_SIZE];
	get_data_frm_disk(SUPER_BLOCK, data);
	convert_string_to_union(data, (void*)(&(block.super)), sizeof(struct rfs_superblock));
	if(block.super.magic_num != SUPER_MAGIC){
		return 0;
	}
	return 1;
}

int rfs_create(unsigned long int ctime, int type){
	
	/*
		Create the new inode and write it to disk.
	*/

	if(disk_mounted == FALSE || disk == NULL){
		return 0;
	}

	union rfs_block block;
	int inode_num = generate_new_inode_num();
	int block_num, inode_index;
	char data[BLK_SIZE];
	seek_inode_block_inode_num(inode_num, &block_num, &inode_index);
	if(inode_index == 0){
		// Create a new inode and then save it.
		block.inode[num_inode].isvalid = 1;
		block.inode[num_inode].allocated = 0;
		block.inode[num_inode].inode_num = inode_num;
		//fprintf(stderr, "DEBUG:%d\n", block.inode[num_inode].inode_num);
		block.inode[num_inode].tstamp.ctime = ctime;
		block.inode[num_inode].tstamp.atime = 0;
		block.inode[num_inode].tstamp.mtime = 0;
		block.inode[num_inode].type = type;
		if(type == DIR_TYPE){
			block.inode[num_inode].size = BLK_SIZE;
		}
		else{
			block.inode[num_inode].size = 0;
		}
		manage_inode_counters();
		// char data[BLK_SIZE];
		if(!write_data_to_disk(block_num, (void *)&(block.inode), sizeof(union rfs_block))){
			return 0;
		}
	}
	else{
		struct rfs_inode temp;
		temp.isvalid = 1;
		temp.allocated = 0;
		temp.type = type;
		temp.inode_num = inode_num;
		temp.tstamp.ctime = ctime;
		temp.tstamp.atime = 0;
		temp.tstamp.mtime = 0;
		// Donot create a new one rather read the disk get the inodes create there and save that.
		get_data_frm_disk(block_num, data);
		convert_string_to_union(data, (void *)(&(block.inode)), sizeof(union rfs_block));
		if(type == DIR_TYPE){
			temp.size = BLK_SIZE;
		}
		else{
			temp.size = 0;
		}
		manage_inode_counters();
		memcpy(block.inode + inode_index, &temp, sizeof(struct rfs_inode));
		if(!write_data_to_disk(block_num, (void *)&(block.inode), sizeof(union rfs_block))){
			return 0;
		}
	}
	return inode_num;
}


int rfs_delete(int inode_num){
	/*
		deletes the data held by an inode and resets
		for use,updates free block bitmap
	*/

	if(disk_mounted == FALSE || disk == NULL){
		return 0;
	}
	//getting location of inode
	int block_number;
	int inode_index;
	seek_inode_block_inode_num(inode_num,&block_number,&inode_index);
	//fetching the required inode
	char buffer[BLK_SIZE];
	union rfs_block block;
	get_data_frm_disk(block_number,buffer);
	convert_string_to_union(buffer,(void *)&block.inode,sizeof(union rfs_block));	
	struct rfs_inode *current_inode = &(block.inode[inode_index]); //pointer to the required inode
	//checking if a valid inode or not
	if(!is_set(current_inode->isvalid)){
		fprintf(stderr,"Cannot delete. File does not exist.\n");
		return 0;
	}
	for(int i=0;i<current_inode->allocated;i++){
		//setting all the allocated blocks to usable now
		free_disk_block(current_inode->direct[i]);
	}
	current_inode->isvalid = 0;
	return 1;
}

void rfs_inode_debug(int inode_num){

	/*
		For Debugging inodes using inode number.
	*/

	if(disk_mounted == FALSE || disk == NULL){
		return ;
	}

	int block_num , inode_index;
	seek_inode_block_inode_num(inode_num, &block_num, &inode_index);
}

int rfs_getsize(int inode_num){
	
	/*
		returns the logical size of the inode in bytes
	*/

	if(disk_mounted == FALSE || disk == NULL){
		return 0;
	}

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
//function to return the file attributes
int rfs_getattr(int inode_num,char *data){
	/*
	*writes the required inode information to the buffer
	*Function will return the inode number, size of the file in bytes
	*timestamp data type of file and blocks in use.
	*/
	if(data == NULL){
		fprintf(stderr,"Invalid buffer to write to.\n");
		return 0;
	}	
	int block_number; //inode block number
	int inode_index; //offset of inode in block
	seek_inode_block_inode_num(inode_num,&block_number,&inode_index); //sets the value of the previous fields
	
	char buffer[BLK_SIZE];
	union rfs_block block;
	get_data_frm_disk(block_number,buffer);
	convert_string_to_union(buffer,(void *)&block.inode,sizeof(union rfs_block)); //gets the required inode block
	struct rfs_inode *current_inode = &(block.inode[inode_index]); //pointer to the required inode

	//check if the inode is valid
	if(!is_set(current_inode->isvalid)){
		fprintf(stderr,"Invalid inode accessed. Inode: %d is not in use\n",inode_num);
		return 0;
	}

	//extract the fields of the inode
	int c_ptr = 0;
	memcpy(data + c_ptr,&inode_num,sizeof(int)); //writing the inode number into the buffer
	c_ptr += sizeof(int);

	memcpy(data + c_ptr,&(current_inode->size),sizeof(int)); // writing the size of the file in bytes to the buffer
	c_ptr += sizeof(int);

	memcpy(data + c_ptr,&(current_inode->tstamp),sizeof(struct timestamp)); //writes the timestamp to the buffer
	c_ptr += sizeof(struct timestamp);

	memcpy(data + c_ptr,&(current_inode->type),sizeof(int)); //writes the type of the file to the buffer
	c_ptr += sizeof(int);

	memcpy(data + c_ptr,current_inode->direct,sizeof(POINTERS_PER_INODE * sizeof(int)));	
	c_ptr += sizeof(POINTERS_PER_INODE * sizeof(int));
	return 1;
}

//read data of 'length' bytes from an offset from an inode
int rfs_read(int inode_num,char *data,int length,int offset){
	
	/*NOTE:
	* Assumes that the direct pointers are written one after the other
	* i.e if direct[0] is full the next data is in direct[1] and so on.
	*/

	if(data == NULL){
		fprintf(stderr,"Invalid buffer provided\n");
		return 0;
	}
	if(length > (DATA_SIZE)*(POINTERS_PER_INODE)){
		fprintf(stderr,"Too Big data\n");
		return 0;
	}

	//function will read 'length' amount of bytes into the buffer data
	int block_index; //which inode block
	int inode_index; //offset from the inode block
	union rfs_block block, datablock; //union to hold the block's contents
	seek_inode_block_inode_num(inode_num,&block_index,&inode_index); //gets the block number and inode offset	
	
	//set the fields of the block union
	char buffer[BLK_SIZE]; 
	get_data_frm_disk(block_index,buffer); //get data from the block
	convert_string_to_union(buffer,(void *)&block.inode,sizeof(union rfs_block)); //convert it back inode format.
	/* 
	* POSSIBLE BUGS: memcpy may not have copied all the fields of the inode 
	* struct correctly
	* UPDATE: revised, using a pointer instead, so memcpy is now unnecessary
	*/
	struct rfs_inode *current_inode = &(block.inode[inode_index]); //pointer to the required inode	
	//first step is to check if valid bit is set
	if(!is_set(current_inode->isvalid)){
		fprintf(stderr,"Invalid inode to read from. Inode number: %d is not in use.\n",inode_num);
		return 0;
	}
	/*CURRENTLY IGNORING OFFSET
	* FIX IN LATER REVISIONS
	*/

	//now to read 'length' bytes from an offset(?) from the direct pointers
	int c_ptr = 0; //offset to read into in destination buffer
	const int fetch_size = (BLK_SIZE)- sizeof(int); //the max amount of bytes each data block can hold
	int blocks_to_read_from = length / fetch_size; //gets the number of blocks to be read from
	int loop_var = 0; //loop variable to iterate over the required blocks to read from
	do{
		int bytes_read;
		get_data_frm_disk(current_inode->direct[loop_var],buffer);
		convert_string_to_union(buffer,(void *)&datablock.buffer,sizeof(union rfs_block));
		if(length >= fetch_size){
			bytes_read = fetch_size;
		}
		else{
			bytes_read = length % fetch_size;
		}
		memcpy(data + c_ptr,datablock.buffer,bytes_read);
		length -= bytes_read;
		c_ptr += bytes_read;
		loop_var ++;
	}
	while(loop_var < blocks_to_read_from);
	/* 
	 *INSERT CODE TO UPDATE TIMESTAMP HERE
	*/
	unsigned long int seconds = time(NULL); // To get the current time
	current_inode->tstamp.atime = seconds;
	if(!write_data_to_disk(block_index, (void *)&(block.inode), sizeof(union rfs_block))){
		return 0;
	}
	return 1;	
}

//write data of 'length' bytes from an offset
int rfs_write(int inode_num,char *data,int length,int offset){
	
	/*NOTE:
	* Assumes that the direct pointers are written one after the other
	* i.e if direct[0] is full the next data is in direct[1] and so on.
	*/
	if(disk_mounted == FALSE || disk == NULL){
		return 0;
	}
	if(length > (DATA_SIZE)*(POINTERS_PER_INODE)){
		fprintf(stderr,"Too Big data\n");
		return 0;
	}

	if(data == NULL){
		fprintf(stderr,"Invalid buffer provided\n");
		return 0;
	}

	if(data == NULL){
		fprintf(stderr,"Invalid buffer provided\n");
		return 0;
	}

	//function will write 'length' amount of bytes from the buffer data
	int block_index; //which inode block
	int inode_index; //offset from the inode block
	union rfs_block block, datablock; //union to hold the block's contents
	seek_inode_block_inode_num(inode_num,&block_index,&inode_index); //gets the block number and inode offset	
	
	//set the fields of the block union
	char buffer[BLK_SIZE]; 
	get_data_frm_disk(block_index,buffer); //get data from the block
	convert_string_to_union(buffer,(void *)(&(block.inode)),sizeof(union rfs_block)); //convert it back inode format.
	/* 
	* POSSIBLE BUGS: memcpy may not have copied all the fields of the inode 
	* struct correctly
	* UPDATE: revised, using a pointer instead, so memcpy is now unnecessary
	*/
	struct rfs_inode *current_inode = &(block.inode[inode_index]); //pointer to the required inode	
	//first step is to check if valid bit is set
	if(!is_set(current_inode->isvalid)){
		fprintf(stderr,"Invalid inode to write from. Inode number: %d is not in use.\n",inode_num);
		return 0;
	}
	/*CURRENTLY IGNORING OFFSET
	* FIX IN LATER REVISIONS
	*/

	//now to read 'length' bytes from an offset(?) from the direct pointers
	int c_ptr = 0; //offset to write into in destination buffer
	const int fetch_size = (BLK_SIZE)- sizeof(int); //the max amount of bytes each data block can hold
	int blocks_to_write_into = length / fetch_size; //gets the number of blocks to write into
	int loop_var = (current_inode->allocated < POINTERS_PER_INODE) ? current_inode->allocated : -1; //loop variable to iterate over the required blocks to write into
	
	do{
		if(loop_var == -1){
			fprintf(stderr,"Data cannot be written to this file.No free blocks\n");
			return 0;
		}
		int bytes_written;
		int next_block_num = current_inode->direct[current_inode->allocated];
		// Check if there is any direct block associated with inode.
		if(current_inode->direct[0] < NUM_INODE_BLOCKS+1 || current_inode->direct[0] > NUM_BLOCKS){
			next_block_num = get_next_free_disk_block_num();
			current_inode->direct[loop_var] = next_block_num;
		}
		if(length >= fetch_size){
			bytes_written = fetch_size;
		}
		else{
			bytes_written = length % fetch_size;
		}
		union rfs_block d_block;
		get_data_frm_disk(current_inode->direct[loop_var], d_block.buffer);
		if(strlen(d_block.buffer) > DATA_SIZE){
			next_block_num = get_next_free_disk_block_num();
			loop_var = (current_inode->allocated += 1);
			current_inode->direct[loop_var] = next_block_num;
		}
		else{
			if(d_block.buffer[0] == '\0'){
				memcpy(d_block.buffer, data + c_ptr, bytes_written);
			}
			else{
				int i = 0;
				int count = 0;
				do{
					char chr = d_block.buffer[i];
					if(chr == '.' || (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z')){
						count++;
					} 
					else{
						break;
					}
					i += bytes_written;
				}while(i < DATA_SIZE);
				memset(d_block.buffer + (bytes_written)*count, '\0', (BLK_SIZE - (sizeof(int))));
				memcpy(d_block.buffer+bytes_written*count, data + c_ptr, bytes_written);
				// memset(d_block.buffer + (bytes_written)*(count+1), '\0', (BLK_SIZE - (sizeof(int))));
			}
		}
		write_data_to_disk(next_block_num,(void *)(&(d_block.buffer)),sizeof(union rfs_block));
		c_ptr += bytes_written;
		length -= bytes_written;
		loop_var++;
	}
	while(loop_var < blocks_to_write_into);
	/* 
	 *INSERT CODE TO UPDATE TIMESTAMP HERE
	*/
	unsigned long int seconds = time(NULL); // To get the current time
	current_inode->tstamp.atime = seconds;
	current_inode->tstamp.mtime = seconds;
	if(!write_data_to_disk(block_index, (void *)&(block.inode), sizeof(union rfs_block))){
		return 0;
	}
	return 1;
}

int rfs_unmount(){
	
	/*
		Unmounts the File System.
	*/

	if(disk_mounted == FALSE || disk == NULL){
		return 0;
	}

	disk_unmount();
	if(bitmap != NULL){
		free(bitmap);
		bitmap = 0x00;
	}
	delete_disk();
	return 1;
}



