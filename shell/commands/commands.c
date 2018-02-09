/* USP File System Project RFileSystem
    author: rishabh
            rajat
            ravi
*/
#include "commands.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../vrfs/vrfs.h"

extern int dentry_index;

int list(){
    read_dir(".");
    if(dentry_index == 0 || dirdata == NULL){
        printf("\n");
        return 0;
    }

    for(int i = 0;i < dentry_index;i++){
        printf("%s\t", dirdata[i].name);
    }
    printf("\n");
    return 1;
}

int touch(char* file){
    if(!create(file)){
        return 0;
    }
    return 1;
}

int stat_file(char *file){
    // printf("%s\n", file);
    struct vrfs_stat *filestat = stat(file);
    if(filestat == NULL){
        return 0;
    }
    printf("File Stat for: %s\n", file);
    printf("    Inode Number : %d\n", filestat->inode_num);
    printf("    Size : %d\n", filestat->size);
    if(filestat->type == 0x01){
        printf("    Regular File\n");
    }
    else{
        printf("    Directory\n");
    }
    printf("    Create Time : %ld\n", filestat->tstamp.ctime);
    printf("    Access Time : %ld\n", filestat->tstamp.atime);
    printf("    Modified Time : %ld\n", filestat->tstamp.mtime);
    return 1;
}

void current_working_dir(){
    printf("%s\n", present_working_directory());
    return ;
}

int mkdir(char* dirname){
    if(!make_directory(dirname)){
        return 0;
    }
    return 1;
}

int cd(char* dirname){
    if(!change_directory(dirname)){
        return 0;
    }
    return 1;
}