#include<stdio.h>
#include"memdisk/memdisk.h"
#include"rfs/rfs.h"

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
	for(int i = 0;i <= 50;i++){
		rfs_create(i+50);
	}
	//rfs_delete(1);
	//rfs_delete(2);
	//rfs_delete(3);
	printf("Size of file: %d \n", rfs_getsize(1));
	printf("Size of file: %d \n", rfs_getsize(3));
	rfs_debug();
	rfs_unmount();
	rfs_format();
	return 0;
}

