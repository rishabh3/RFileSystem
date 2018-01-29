/*
 * rfs.c
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#include <string.h>
#include <rfs.h>

// Utility function to convert union to string.
void convert_struct_to_string(union rfs_block *block_data, char* data){
	memcpy(data, block_data, sizeof(union rfs_block));
}

/* scan a mounted fs and prints info on superblock and each inode */
void rfs_debug(){

}

//writes a new file system onto the disk,re writes super block
int rfs_format(){
	return 0;
}

//checks if disk has filesystem, if present read the super block and build free block bitmap
int rfs_mount(){
	return 0;
}

//creates a new inode of zero length
int rfs_create(){
	return 0;
}

/*deletes the data held by an inode and resets for use,updates free block bitmap */
int rfs_delete(int inode_num){
	return 0;
}

//returns the logical size of the inode in bytes
int rfs_getsize(int inode_num){
	return 0;
}

//read data of 'length' bytes from an offset from an inode
int rfs_read(int inode_num,char *data,int length,int offset){
	return 0;
}

//write data of 'length' bytes from an offset
int rfs_write(int inode_num,char *data,int length,int offset){
	return 0;
}



