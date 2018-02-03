#include <stdio.h>
#include "../memdisk/memdisk.h"
#include "../rfs/rfs.h"
#include "../vrfs/vrfs.h"

void test_rfs_create(){
    rfs_create(1234, F_TYPE);
}

void test_mkfs(){
    make_rfs("rdx");
}

int main(){
    disk_init();
    rfs_format();
    // rfs_mount();
    // test_rfs_create();
    test_mkfs();
    rfs_debug();
    rfs_unmount();
    return 0;
}