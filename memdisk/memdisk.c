/*
 * memdisk.c
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include "memdisk.h"


static int num_blocks = 0; // This will increase when the block is completely full
static int nreads = 0; // This will keep track of number of reads.
static int nwrites = 0; // This will keep track of number of writes to disk.


void sanity_check(int block_index, char * data, int type_of_operation){

	/*																*
	 * Sanity Check functionality for error checking for parameters.*
	 * 																*/

	if(block_index < 0){
		printf("ERROR: block number (%d) is less than 0", block_index);
		abort();
	}
	if(block_index >= NUM_BLOCKS){
		printf("ERROR: block number (%d) is greater than disk capacity", block_index);
		abort();
	}
	if(WRITE && !data){
		printf("ERROR: null pointer to data!\n");
		abort();
	}
}

void disk_init(){
	disk = (struct memdisk *)malloc(sizeof(struct memdisk)*NUM_BLOCKS);
	for(int i = 0;i < NUM_BLOCKS;i++){
		disk->in_use = FREE;
	}
	new_disk = TRUE;
}

void disk_mount(){
	disk_mounted = TRUE;
}

int  reset_stats(){
	// reset stats of the disk
	num_blocks = 0;
	nreads = 0;
	nwrites = 0;
	return 1;
}

void disk_unmount(){
	// Unmount the disk
	disk_mounted = FALSE;
}

void delete_disk(){
	// Delete the disk free up the memory space
	if(!disk){
		return ;
	}
	free(disk);
	disk = 0x00;
	reset_stats();
}

int disk_size(){

	/* Return Remaining disk size*/

	return num_blocks;
}

void disk_read(int block_num, char * data){

	/* Disk Read fuctionality */

	sanity_check(block_num, data, READ);
	if(disk[block_num].in_use != IN_USE){
		printf("ERROR: Reading the disk at block (%d).\n", block_num);
		abort();
	}
	else{
		memcpy(data, disk[block_num].disk_data, sizeof(disk[block_num].disk_data));
		nreads++;
	}
}

int disk_write(int block_num, char* data){

	/* Disk Write Functionality */

	sanity_check(block_num, data, WRITE);
	memcpy(disk[block_num].disk_data, data, sizeof(disk[block_num].disk_data));
	disk[block_num].in_use = IN_USE;
	nwrites++;
	return 1;
}




