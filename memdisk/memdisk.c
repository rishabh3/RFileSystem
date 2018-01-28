/*
 * memdisk.c
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include "memdisk.h"

struct memdisk memdisk[];

static int num_blocks = 0; // This will increase when the block is completely full
static int nreads = 0; // This will increase when the inode block is filled and then reset when one block is completely filled
static int nwrites = 0;

void sanity_check(int block_index, union block * data, int type_of_operation){
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

int disk_size(){
	return num_blocks;
}

void disk_read(int block_num, char * data){
	sanity_check(block_num, data, READ);
	memcpy(data, memdisk[block_num].disk_data, strlen(memdisk[block_num].disk_data));
	nreads++;
}

void disk_write(int block_num, char* data){
	sanity_check(block_num, data, WRITE);
	memcpy(memdisk[block_num].disk_data, data, strlen(data));
	nwrites++;
}




