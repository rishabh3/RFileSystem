/*
 * disk.c
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


#include "disk.h"

static int num_blocks = 0; // This will increase when the block is completely full
static int nreads = 0; // This will keep track of number of reads.
static int nwrites = 0; // This will keep track of number of writes to disk.
static int num_data_block = NUM_INODE_BLOCKS+1; // start of data block

void sanity_check(int block_index, char * data){

	/*																*
	 * Sanity Check functionality for error checking for parameters.*
	 * 																*/

	if(block_index < 0){
		printf("ERROR: block number (%d) is less than 0", block_index);
		return;
	}
	if(block_index >= NUM_BLOCKS){
		printf("ERROR: block number (%d) is greater than disk capacity", block_index);
		return;
	}
	if(!data){
		printf("ERROR: null pointer to data!\n");
		return;
	}
}

void disk_init(char *filename, char *mode){
	/*
		Initialize a new disk by allocating some memory and setting up the flags.
	*/
	if(disk == NULL){
		disk = fopen(filename, mode);
		memcpy(file_name, filename, strlen(filename));
		file_name[strlen(filename)] = '\0';	
	}
	else{
		fclose(disk);
		disk = NULL;
		disk = fopen(filename, mode);
	}
    if(!disk) {
		fprintf(stderr, "Failed to initialize the disk!\n");
		return ;
	}
	struct stat bf;
	if(stat(fileno(disk), &bf) != 0){
		fprintf(stderr, "Failed to initialize the disk!\n");
		fclose(disk);
		disk = NULL;
		return ;
	}
	if(bf.st_size == 0)
    	ftruncate(fileno(disk), NUM_BLOCKS*DISK_BLK_SIZE);

    num_blocks = NUM_BLOCKS;
    
}

void disk_mount(){
	/*
		Set the flag to indicate that disk has been mounted and its ready for use.
	*/
	disk_mounted = TRUE;
}

int  reset_stats(){
	/*
		Reset stats maintained for the disks.
	*/
	num_blocks = 0;
	nreads = 0;
	nwrites = 0;
	num_data_block = NUM_INODE_BLOCKS + 1;
	return 1;
}

void disk_unmount(){
	/*
		Unmount the disk. Disk is not in use.
	*/
	disk_mounted = FALSE;
}

void disk_close(){
	/*
		Closes the disk.
	*/
	if(!disk){
		return ;
	}
    fprintf(stdout, "%d disk block reads\n",nreads);
	fprintf(stdout, "%d disk block writes\n",nwrites);
	fclose(disk);
	disk = NULL;
	reset_stats();
}

int disk_size(){

	/* 
		Return Remaining disk size
	*/

	return num_blocks;
}

void disk_read(int block_num, char * data){

	/* 
		Disk Read fuctionality
    */

	sanity_check(block_num, data);
	
    fseek(disk, block_num*DISK_BLK_SIZE, SEEK_SET);

    if(fread(data, DISK_BLK_SIZE, 1, disk) == 1){
        nreads++;
    }
    else{
        fprintf(stderr, "Error: reading from the disk at the disk block number %d\n", block_num);
    }
}

int disk_write(int block_num, char* data){

	/* 
		Disk Write Functionality
	*/

	sanity_check(block_num, data);
	
    fseek(disk, block_num*DISK_BLK_SIZE, SEEK_SET);

    if(fwrite(data, DISK_BLK_SIZE, 1, disk)){
        nwrites++;
        num_blocks--;
    }
    else{
        fprintf(stderr, "Error: writing to the disk at the disk block number %d\n", block_num);
        return 0;
    }
    return 1;
}







