#include "../vrfs/vrfs.h"
#include "../rfs/rfs.h"
#include "../memdisk/memdisk.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
void convert_struct_to_string(char * data, struct dentry * direntry, unsigned long int size){
    memcpy(data, direntry, size);
    strcat(data, "\0");
}

void convert_string_to_struct(struct dentry * direntry, char * data ,  unsigned long int size){
    memcpy(direntry, data, size);
    //data[size] = '\0';
}
*/
int main(){
/*
    struct dentry obj1, obj2, obj3, obj4;
    obj1.inode_num = 1;
    memcpy(obj1.name, "rdx", 4);
    obj2.inode_num = 2;
    memcpy(obj2.name, "test", 5);
    char data[1024];
    convert_struct_to_string(data, &obj1, sizeof(struct dentry));
    convert_struct_to_string(data + sizeof(struct dentry), &obj2, sizeof(struct dentry));
    convert_string_to_struct(&obj3, data, sizeof(struct dentry));
    convert_string_to_struct(&obj4, data + sizeof(struct dentry), sizeof(struct dentry));
    printf("%s %d\n", obj3.name, obj3.inode_num);
    printf("%s %d\n", obj4.name, obj4.inode_num);
*/  
    disk_init();
    rfs_format();
    make_rfs("ravi");
    int inode = create("sample");
    printf("sadasd\n");
   
    // char data[100];
    // if(!rfs_getattr(inode,data)){
    //     fprintf(stderr,"Failed to get file stats\n");
    //     rfs_unmount();
    //     exit(1);
    // }
    
    struct vrfs_stat *temp = stat("sample");
    printf("%d %d %d %d\n",temp->inode_num,temp->size,temp->type,temp->direct[0]);
    rfs_unmount();
    return 0;
}