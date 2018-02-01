#include<stdio.h>
#include"memdisk/memdisk.h"
#include"rfs/rfs.h"
#include"vrfs/vrfs.h"
#include"shell/commands/commands.h"


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
	// for(int i = 0;i < NUM_BLOCKS;i++){
	// 	printf("%d:%d\n", i, disk[i].in_use);
	// }
	// char str[6] = "Hello\0";
	// char msg[3] = "Hi\0";
	// disk_write(1, msg);
	// disk_write(1, str);
	// char rcv[6];
	// disk_read(1, rcv);
	// printf("%s\n", rcv);
	make_rfs("rdx");
	// rfs_create(1234, 1);
	// printf("The disk has been mounted!\n");
	//struct rfs_inode inode = read_data_frm_disk(1);
	// char data[BLK_SIZE];
	// printf("%d\n",inode.inode_num);
	// printf("%d\n", inode.direct[0]);
	// rfs_write(1,"Hello My Name is Rishabh.\0", 27, 0);
	// rfs_read(1, data, BLK_SIZE,0);
	// printf("%s\n", data);
	touch("test");
	// printf("File created\n");
	list("rdx");
	// for(int i = 0;i < 48;i++){
	// 	rfs_create(i+48);
	// }
	//rfs_delete(1);
	//rfs_delete(2);
	//rfs_delete(3);
	//printf("Size of file: %d \n", rfs_getsize(1));
	//printf("Size of file: %d \n", rfs_getsize(3));
	rfs_debug();
	rfs_unmount();
	//rfs_format();
	return 0;
}

