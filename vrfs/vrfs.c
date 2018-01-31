/*
 * vrfs.c
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#include "vrfs.h"
#include "../rfs/rfs.h"
#include <time.h>


void convert_struct_to_string(char * data, struct dentry * direntry, unsigned long int size){
    memcpy(data, direntry, size);
}

int make_rfs(char *username){
    unsigned long int seconds;
    seconds = time(NULL);
    int inode_num;
    if(rfs_mount()){
        // Create a directory for the user
        inode_num = rfs_create(seconds, 0, 0, DIR_TYPE);
        // Name of the directory is username
        // Create a dentry structure
        struct dentry rootdir;
        rootdir.inode_num = inode_num;
        memcpy(rootdir.name, username, MAX_SIZE);
        rootdir.name[MAX_SIZE-1] = "\0";
        char data[BLK_SIZE];
        convert_struct_to_string(data, &rootdir, sizeof(struct dentry));
        // Write the data to disk
        if(!rfs_write(inode_num, data, strlen(data), 0)){
            return 0;
        }
        // Change the directory to that directory.
        memcpy(rootdirname, username, MAX_SIZE);
        strcat(rootdirname, "/");
        memcpy(current_working_directory, rootdirname, MAX_SIZE);
        rootinode = inode_num;
        return 1;
    }
    return 0;
}



int open(char *filename){
    unsigned long int seconds;
    seconds = time(NULL);
    int inode_num;
    int parent_inode_num = rootinode;
    char *cwd;
    //Get the current Directory.
    cwd = present_working_directory();
    //Create a Inode for the file.
    inode_num = rfs_create(seconds, 0,0,F_TYPE);
    //Add the dentry data to the data of the current directory.
    struct dentry newfile;
    newfile.inode_num = inode_num;
    memcpy(newfile.name, filename, MAX_SIZE);
    newfile.name[MAX_SIZE-1] = "\0";
    //Return 
}

char * present_working_directory(){
    return current_working_directory;
}

