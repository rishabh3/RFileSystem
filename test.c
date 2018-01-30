#include<stdio.h>
#include"memdisk/memdisk.h"
#include"rfs/rfs.h"

int main(){
    printf("Program to test the functionality of RFS\n");
    disk_init();
    rfs_format();
    rfs_mount();
    rfs_unmount();
    rfs_format();
   // rfs_debug();
    return 0;
}