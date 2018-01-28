/*
 * memdisk.c
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include "memdisk.h"

struct memdisk memdisk[];
static int num_blocks = 0; // This will increase when the block is completely full
static int num_inode_blocks = 0; // This will increase when the inode block is filled and then reset when one block is completely filled


void sanity_check(int block_index, union block * data, int type_of_operation){
	if(block_index < 0){
		printf("ERROR: block number (%d) is less than 0", block_index);
		abort();
	}
	if(block_index >= NUM_BLOCKS){
		printf("ERROR: block number (%d) is greater than disk capacity", block_index);
		abort();
	}
	if(memdisk[block_index].in_use == FULL){
		printf("ERROR: block number (%d) is full", block_index);
		abort();
	}
	if(type_of_operation == WRITE && !data){
		printf("ERROR: null pointer to data!\n");
		abort();
	}
}

int disk_size(){
	return num_blocks;
}

int get_next_free_inode(){
	return num_inode_blocks;
}

void disk_read(int block_num, union block * data){
	sanity_check(block_num, data, READ);
	data = &memdisk[block_num].block_type;
}

void disk_write(int block_num, union block * data, int type){
	sanity_check(block_num, data, WRITE);
	switch(type){
		case SUPERBLOCK: memdisk[block_num].block_type = *data;
						 memdisk[block_num].in_use = FULL;
						 break;
		case INODE: memdisk[block_num].block_type = *data;
					break;
		case DATA:  break;
		default:	printf("ERROR: data type not found!\n");
					break;
	}

}




