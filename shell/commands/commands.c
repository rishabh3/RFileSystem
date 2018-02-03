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

int list(char* path){
    int size;
    struct dentry *result = (struct dentry *)malloc((MAX_DENTRY)*(sizeof(struct dentry)));
    read_dir(path, &size, result);
    if(size == 0 || result == NULL){
        printf("\n");
        return 0;
    }

    for(int i = 0;i < size;i++){
        printf("%s\t", result[i].name);
    }
    printf("\n");
    free(result);
    result = NULL;
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