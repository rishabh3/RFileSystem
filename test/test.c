#include "../vrfs/vrfs.h"
#include "../rfsdisk/rfsdisk.h"
#include "../disk/disk.h"
#include "../shell/commands/commands.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    disk_init("testdiskfile");
    if(disk != NULL){
        rfs_format();
        make_rfs("rdx");
        touch("testfile");
        mkdir("testdir");
        rfs_unmount();
        make_rfs("rdx");
        list();
        current_working_dir();
        rfs_unmount();
    }
    disk_close();
    return 0;
}