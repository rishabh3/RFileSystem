/*
 * test.c
 *
 *  Created on: 29-Jan-2018
 *      Author: rishabh
 */

#include <stdio.h>
#include "rfs/rfs.h"

int main(){
	printf("Mounting the disk!\n");
	if(!rfs_mount()){
		return -1;
	}
	printf("The disk has been mounted!\n");
	rfs_format();
	printf("The disk has been formatted \n");
	rfs_debug();
	rfs_unmount();
}

