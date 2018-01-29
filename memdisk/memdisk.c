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


void sanity_check(int block_index, union block * data, int type_of_operation){

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

struct memdisk * disk_init(){
	return (struct memdisk *)malloc(sizeof(struct memdisk)*NUM_BLOCKS);
}

int  reset_stats(){
	num_blocks = 0;
	nreads = 0;
	nwrites = 0;
	return 1;
}

void disk_unmount(){
	if(!disk){
		return ;
	}
	free(disk);
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
		memcpy(data, disk[block_num].disk_data, strlen(disk[block_num].disk_data));
		nreads++;
	}
}

void disk_write(int block_num, char* data){

	/* Disk Write Functionality */

	sanity_check(block_num, data, WRITE);
	if(disk[block_num].in_use == IN_USE){
		printf("ERROR: Writing to the disk at block (%d).\n", block_num);
		abort();
	}
	else{
		memcpy(disk[block_num].disk_data, data, strlen(data));
		disk[block_num].in_use = IN_USE;
		nwrites++;
	}
}




