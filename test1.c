/*
 * test1.c
 *
 *  Created on: 30-Jan-2018
 *      Author: risha
 */
#include "memdisk/memdisk.h"
#include "rfs/rfs.h"
#include <stdio.h>

int main(){
		/*union rfs_block block, result;
		block.super.magic_num = SUPER_MAGIC;
		block.super.num_blocks = NUM_BLOCKS;
		block.super.num_inodeblocks = 10;
		block.super.num_inodes = (INODES_PER_BLOCK)*10;
		printf("SuperBlock: \n");
		printf(" 	%d magic number \n", block.super.magic_num);
		printf("	%d blocks\n", block.super.num_blocks);
		printf(" 	%d inode blocks\n", block.super.num_inodeblocks);
		printf(" 	%d inodes \n", block.super.num_inodes);
		char data[4096];
		convert_union_to_string((&(block.super)), data, sizeof(struct rfs_superblock));
		convert_string_to_union(data, (&(result.super)));
		printf("SuperBlock: \n");
		printf(" 	%d magic number \n", result.super.magic_num);
		printf("	%d blocks\n", result.super.num_blocks);
		printf(" 	%d inode blocks\n", result.super.num_inodeblocks);
		printf(" 	%d inodes \n", result.super.num_inodes);*/
	printf("%ld\n", sizeof(union rfs_block));
}

