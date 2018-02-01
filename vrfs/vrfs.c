/*
 * vrfs.c
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#include "vrfs.h"
#include "../rfs/rfs.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



void convert_struct_to_string(char * data, struct dentry * direntry, unsigned long int size){
    memcpy(data, direntry, size);
    strcat(data, "\0");
}

void convert_string_to_struct(struct dentry * direntry, char * data ,  unsigned long int size){
    memcpy(direntry, data, size);
    //data[size] = '\0';
}


int make_rfs(char *username){
    unsigned long int seconds;
    seconds = (unsigned long int)time(NULL);
    int inode_num;
    if(rfs_mount()){
        // Create a directory for the user
        inode_num = rfs_create(seconds,  DIR_TYPE);
        // Name of the directory is username
        // Create a dentry structure
        struct dentry rootdir;
        //fprintf(stderr, "DEBUG:%d\n", inode_num);
        rootdir.inode_num = inode_num;
        memcpy(rootdir.name, ".", 1);
        rootdir.name[1] = '\0';
        char data[BLK_SIZE];
        convert_struct_to_string(data, &rootdir, sizeof(struct dentry));
        // Write the data to disk
        int size = sizeof(struct dentry);
        // struct dentry file;
        // convert_string_to_struct(&file, data, sizeof(struct dentry));
        // fprintf(stderr, "DEBUG: %d %s\n", file.inode_num, file.name);
        if(!rfs_write(inode_num, data, size, 0)){
            return 0;
        }
        // Change the directory to that directory.
        memcpy(rootdirname, username, strlen(username));
        // strcat(rootdirname, "/");
        memcpy(current_working_directory, rootdirname, strlen(username));
        rootinode = inode_num;
        return 1;
    }
    return 0;
}



int create(char *filename){
    unsigned long int seconds;
    seconds = time(NULL);
    int inode_num;
    char data[BLK_SIZE];
    int parent_inode_num = rootinode;
    char *cwd;
    //Get the current Directory.
    cwd = present_working_directory();
    //Create a Inode for the file.
    inode_num = rfs_create(seconds, F_TYPE); // This handles for the file
    //Add the dentry data to the data of the current directory.
    //Read the data of the current directory.
    struct dentry newfile, file;
    newfile.inode_num = inode_num;
    memcpy(newfile.name, filename,strlen(filename));
    newfile.name[strlen(filename)] = '\0';
    // printf("%s \n %d \n", newfile.name, newfile.inode_num);
    convert_struct_to_string(data, &newfile, sizeof(struct dentry));
    // printf("%s\n", data);
    // convert_string_to_struct(&file, data, sizeof(struct dentry));
    // printf("Name: %s\nNum: %d\n", file.name, file.inode_num);
    if(!rfs_write(parent_inode_num,data, sizeof(struct dentry), 0)){
        return 0;
    }
    //Return
    return 1; 
}

char * present_working_directory(){
    return current_working_directory;
}

int read_dir(char *dirname, int* size, struct dentry * result){
    struct dentry dirdata[MAX_DENTRY];
    struct dentry temp;
    int loop_var = 0;
    int current_pos;
    const char splitter[2] = "-";
    char data[BLK_SIZE];
    char *readpointer;
    char *token;
    if(!strcmp(dirname, rootdirname)){
        if(!rfs_read(rootinode, data, BLK_SIZE, 0)){
            return 0;
        }
        readpointer = data;
        while(*readpointer != '\0'){
            current_pos = loop_var*sizeof(struct dentry);
            convert_string_to_struct(dirdata+loop_var, data + current_pos, sizeof(struct dentry));
            readpointer += current_pos;
            loop_var++;
        }
    }
    *size = loop_var;
    for(int i = 0;i < *size;i++){
        memcpy(result+i, dirdata+i, sizeof(struct dentry));
    }
    return 1;
    // return NULL;
}

