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
#include "../../rfsdisk/rfsdisk.h"

extern int dentry_index;

int list(char *dirname){
    if(!read_dir("."))
        read_dir(dirname);
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

int rm_dir(char *dirname){
    if(!remove_directory(dirname)){
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

int rm(char* filename){
	if(!rm_file(filename)){
		return 0;
	}
	return 1;
}

int read_file(char *filename){
    char data[DATA_BLOCK_SIZE];
    int inode_num;
    struct vrfs_stat* statbuf =  stat(filename);
    if(statbuf->type == DIR_TYPE){
        fprintf(stderr, "Cannot read a directory!\n");
        return 0;
    }
    if((inode_num = open(filename)) == -1){
        fprintf(stderr, "No such file!\n");
        return 0;
    }
    if(!readfile(inode_num, data, DATA_BLOCK_SIZE)){
        fprintf(stderr, "Read from file %s failed!\n", filename);
        return 0;
    }
    fprintf(stdout, "%s", data);
    fprintf(stdout, "\n");
    return 1;
}

int cp_file(char *src, char *dest){
    if(!copy(src, dest)){
        return 0;
    }
    return 1;
}

int mv(char *old, char* new){
    if(!re_name(old, new)){
        return 0;
    }
    return 1;
}

int write_file(char* filename, char* data){
    int inode_num = open(filename);
    if(inode_num == -1){
        fprintf(stderr, "No such file!\n");
        return 0;
    }
    if(!writefile(inode_num, data, strlen(data))){
        fprintf(stderr, "Read from file %s failed!\n", filename);
        return 0;
    }
    return 1;
}