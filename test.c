/*
 * test.c
 *
 *  Created on: 29-Jan-2018
 *      Author: rishabh
 */

#include <stdio.h>
#include "rfs/rfs.h"
#include "memdisk/memdisk.h"

int main(){
	disk_init();
	/*union rfs_block block;
	block.super.magic_num = SUPER_MAGIC;
	char data[4096];
	memcpy(data, &(block.super), sizeof(struct rfs_superblock));
	union rfs_block blk;
	memcpy(&(blk.super), data, strlen(data));
	printf("%d\n", blk.super.magic_num);*/
	if(rfs_format()){
		printf("The disk has been formatted \n");;
	}
	printf("Mounting the disk!\n");
	if(!rfs_mount()){
		return -1;
	}
	printf("The disk has been mounted!\n");
	rfs_create();
	rfs_debug();
	rfs_unmount();
	rfs_format();
	return 0;
}

