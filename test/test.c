#include "../vrfs/vrfs.h"
#include "../rfsdisk/rfsdisk.h"
#include "../disk/disk.h"
#include "../shell/commands/commands.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    disk_init("testdiskfile", "r+");
    if(disk != NULL){
        rfs_format();
        make_rfs("rdx");
        touch("testfile");
        write_file("testfile", "Hello I am Rishabh.");
        write_file("testfile", "This is USP project");
        read_file("testfile");
        list("rdx");
    }
    disk_close();
    return 0;
}