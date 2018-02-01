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
    struct dentry * result = read_dir(path, &size);
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