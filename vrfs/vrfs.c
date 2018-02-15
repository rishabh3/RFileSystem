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

int validate_dirname(char *name, int *inodenum){
    if(dentry_index == 0){
        return 0;
    }
    for(int i = 0;i < dentry_index;i++){
        if(!strcmp(name, dirdata[i].name)){
            *inodenum = dirdata[i].inode_num;
            return 1;
        }
    }
    return 0;
}



void convert_struct_to_string(char * data, struct dentry * direntry, unsigned long int size){
    memcpy(data, direntry, size);
    strcat(data, "\0");
}

void convert_string_to_struct(struct dentry * direntry, char * data ,  unsigned long int size){
    memcpy(direntry, data, size);
    //data[size] = '\0';
}

void convert_string_to_stat(struct vrfs_stat *stat,char *data,unsigned long int size){
    memcpy(stat,data,size);
}

int make_rfs(char *username){
    unsigned long int seconds;
    seconds = (unsigned long int)time(NULL);
    int inode_num;
    int t = rfs_mount();
    if(t == 1){
        // Create a directory for the user
        inode_num = rfs_create(seconds,  DIR_TYPE);
        // Name of the directory is username
        // Create a dentry structure
        struct dentry rootdir;
        //fprintf(stderr, "DEBUG:%d\n", inode_num);
        rootdir.inode_num = inode_num;
        memcpy(rootdir.name, ".", 1);
        rootdir.name[1] = '\0';
        memcpy(dirdata+dentry_index, &rootdir, sizeof(struct dentry));
        dentry_index++;
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
        strcat(current_working_directory, "/");
        rootinode = inode_num;
        currentinode = inode_num;
        return 1;
    }
    else if(t == 2){
        memcpy(rootdirname, username, strlen(username));
        memcpy(current_working_directory, rootdirname, strlen(username));
        //  strcat(current_working_directory, "/");
        rootinode = t-1;
        currentinode = t-1;
        return 1;
    }
    return 0;
}



int create(char *filename){
    unsigned long int seconds;
    seconds = time(NULL);
    int inode_num;
    char data[BLK_SIZE];
    int parent_inode_num = currentinode;
    //Create a Inode for the file.
    inode_num = rfs_create(seconds, F_TYPE); // This handles for the file
    //Add the dentry data to the data of the current directory.
    //Read the data of the current directory.
    struct dentry newfile, file;
    newfile.inode_num = inode_num;
    memcpy(newfile.name, filename,strlen(filename));
    newfile.name[strlen(filename)] = '\0';
    if(dentry_index != 0){
        memcpy(dirdata+dentry_index, &newfile, sizeof(struct dentry));
        dentry_index++;
    }
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


int open(char *filename){
    int inode_num;
    if(validate_dirname(filename, &inode_num)){
        return inode_num;
    }
    return -1;
}



char * present_working_directory(){
    return current_working_directory;
}



int read_dir(char *dirname){
    // struct dentry dirdata[MAX_DENTRY];
    int loop_var = 0;
    int current_pos;
    char data[BLK_SIZE];
    int inode_num;
    if(!strcmp(dirname, rootdirname)){
        if(!rfs_read(rootinode, data, BLK_SIZE, 0)){
            return 0;
        }
        // readpointer = data;
        while(data[current_pos] != '\0'){
            current_pos = loop_var*sizeof(struct dentry);
            convert_string_to_struct(dirdata+loop_var, data + current_pos, sizeof(struct dentry));
            // readpointer += current_pos;
            loop_var++;
        }
        currentinode = rootinode;
    }
    else{
        
        if(validate_dirname(dirname, &inode_num)){
            if(!rfs_read(inode_num, data, BLK_SIZE, 0)){
                return 0;
            }
            // readpointer = data;
            while(data[current_pos] != '\0'){
                current_pos = loop_var*sizeof(struct dentry);
                convert_string_to_struct(dirdata+loop_var, data + current_pos, sizeof(struct dentry));
                // readpointer += current_pos;
                loop_var++;
            }
        }
        else{
            return 0;
        }
    }
    dentry_index = loop_var;
    currentinode = inode_num;
    return 1;
    // return NULL;
}

struct vrfs_stat *stat(char *filename){

    //check for a valid filename being passed
    if(filename == NULL){
        fprintf(stderr,"Please specify a valid file name\n");
        return NULL;
    }

    struct vrfs_stat *stat = (struct vrfs_stat*)malloc(sizeof(struct vrfs_stat));

    if(dentry_index == 0){
        fprintf(stderr,"Failed to read directory\n");
        return NULL;
    }   
    else{
        for(int i=0;i<dentry_index;i++){
            if(!strcmp(dirdata[i].name,filename)){
               // printf("%s %s\n", dirdata[i].name, filename); 
               char buffer[sizeof(struct vrfs_stat)]; 
               rfs_getattr(dirdata[i].inode_num,buffer);
               convert_string_to_stat(stat,buffer,sizeof(buffer));
               return stat;
            }
        }
        fprintf(stderr,"No such file or directory exists\n");
        return NULL;
    }
}

//implementing the read system call
bool read(char *filename,char *data,size_t size){
    //checking if a filename is provided    
    if(filename == NULL){
        fprintf(stderr,"Please specify a filename.\n");
        return FALSE;
    }
    //checking if a buffer is provided
    if(data == NULL){
        fprintf(stderr,"Please provide a valid buffer to write into.\n");
        return FALSE;
    }
    //fetching the required inode.
    struct dentry result[MAX_DENTRY];
    int len;
    read_dir(current_working_directory,&len,result);
    if(len == 0){
        fprintf(stderr,"Failed to read directory\n");
        return FALSE;
    }
    else{
        for(int i=0;i<len;i++){
            if(!strcmp(result[i].name,filename)){
                if(!rfs_read(result[i].inode_num,data,size,0)){
                    fprintf(stderr,"Error in read(). rfs_read exited unsuccessfully\n");
                    return FALSE;
                }
                return TRUE;
            }
        }
        fprintf(stderr,"Filename does not match any valid inode.\n");
        return FALSE;
    }
}

//implementing the write system call
bool write(char *filename,char *data,size_t size){
    //checking if filename is provided
    if(filename == NULL){
        fprintf(stderr,"Please specify a filename to use.\n");
        return FALSE;
    }
    //checking if a valid buffer is provided
    if(data == NULL){
        fprintf(stderr,"Please provide a valid buffer to read from\n");
        return FALSE;
    }
    //obtaining the required inode
    struct dentry result[MAX_DENTRY];
    int len;
    read_dir(current_working_directory,&len,result);
    if(len == 0){
        fprintf(stderr,"Failed to read directory\n");
        return FALSE;
    }
    else{
        for(int i=0;i<len;i++){
            if(!strcmp(result[i].name,filename)){
                if(!rfs_write(result[i].inode_num,data,size,0)){
                    fprintf(stderr,"Error in write(). rfs_write exited unsuccessfully\n");
                    return FALSE;
                }
                return TRUE;
            }
        }
        fprintf(stderr,"Filename does not match any valid inode.\n");
        return FALSE;
    } 
}
//function to remove a file from the fs. returns 1 if successful, 0 if failed
bool rm_file(char *filename){
    //check for a filename provided
    if(filename == NULL){
        fprintf(stderr,"Please specify a valid filename.\n");
        return FALSE;
    }
    /*
    * deletion policy: utilize the underlying rfs_delete function
    * basically we find the inode and set the isvalid = 0
    * scan through the blocks and set the inuse bits to 0
    */
    int size;
    struct dentry result[MAX_DENTRY];
    read_dir(current_working_directory,&size,result);
    if(!size){
        fprintf(stderr,"Failed to read directory.\n");
        return FALSE;
    }
    else{
        for(int i=0;i<size;i++){
            if(!strcmp(result[i].name,filename)){
                if(!rfs_delete(result[i].inode_num)){
                    fprintf(stderr,"Failed to delete file: rfs_delete failed.\n");
                    return FALSE;
                }
                return TRUE;
            }
        }
        fprintf(stderr,"Failed to delete file: %s does not exist.\n",filename);
        return FALSE;
    }
}

int get_inode_num_name(){
    return currentinode;
}

int make_directory(char* dirname){
    struct dentry newdir;
    int numb_inode;
    char data[BUFFER_SIZE];
    unsigned long int seconds;
    seconds = time(NULL);
    newdir.inode_num = rfs_create(seconds, DIR_TYPE);
    int length = strlen(dirname);
    memcpy(newdir.name, dirname, length);
    newdir.name[length] = '\0';
    memcpy(dirdata+dentry_index, &newdir, sizeof(struct dentry));
    dentry_index++;
    convert_struct_to_string(data, &newdir, sizeof(struct dentry));
    int inode_num = get_inode_num_name();
    numb_inode = inode_num;
    // Write to current directory the new entry.
    if(!rfs_write(inode_num,data, sizeof(struct dentry), 0)){
        return 0;
    }
    memset(data, '\0', sizeof(data));
    memset(newdir.name, '\0', MAX_SIZE);
    inode_num = newdir.inode_num;
    memcpy(newdir.name, ".", 1);
    newdir.name[1] = '\0';
    convert_struct_to_string(data, &newdir, sizeof(struct dentry));
    if(!rfs_write(inode_num, data, sizeof(struct dentry), 0)){
        return 0;
    }
    memset(data, '\0', BUFFER_SIZE);
    memset(newdir.name, '\0', MAX_SIZE);
    // inode_num = numb_inode;
    newdir.inode_num = numb_inode;
    memcpy(newdir.name, "..", 2);
    newdir.name[2] = '\0';
    convert_struct_to_string(data, &newdir, sizeof(struct dentry));
    if(!rfs_write(inode_num, data, sizeof(struct dentry), 0)){
        return 0;
    }
    return 1;
}


// Some helper functions
char* get_last_dir_in_path(char* path){
    char *last_dir = (char *)malloc(sizeof(char)*MAX_SIZE);
    char* dir = strtok(path, "/");
    while(dir != NULL){
        dir = strtok(NULL, "/");
        if(dir != NULL){
            memset(last_dir, '\0', MAX_SIZE);
            memcpy(last_dir, dir, strlen(dir));
        }
    }
    return last_dir;
}

void append_to_current_working_dir(char *dirname){
    char* cwd = present_working_directory();
    strcat(cwd, dirname);
    strcat(cwd, "/");
}

void modify_current_working_dir(){
    char* cwd = present_working_directory();
    int pos = -1;
    for(int i = strlen(cwd) - 1;i >= 0;i--){
        if(cwd[i] == '/'){
           pos++;     
        }
        if(pos == 1){
            pos = i+1;
            break;
        }
    }
    int len = strlen(cwd) - pos;
    memset(cwd+pos, '\0', len+1);
}

int change_directory(char* dirname){
    /*
        Changes the directory to another directory. 
    */
    if(!strcmp(dirname, ".")){
        return 1;
    }
    char pathbuf[MAX_PATH_SIZE];
    char *cwd = present_working_directory();
    int inode_num;
    memset(pathbuf, '\0', MAX_PATH_SIZE);
    memcpy(pathbuf, cwd, strlen(cwd));

    char* last_dir = get_last_dir_in_path(pathbuf);
    if(!strcmp(rootdirname, last_dir)){
        // Perform movement for the root
        if(currentinode == rootinode){
            // No need to call readdir
            // check if dirname in dirdata.
            if(validate_dirname(dirname, &inode_num)){
                append_to_current_working_dir(dirname);
                read_dir(dirname);
            }
            else{
                fprintf(stderr, "No such Directory!\n");
                return 0;
            }
        }
    }
    else{
        if(!strcmp(dirname, "..")){
            if(currentinode == rootinode){
                return 0;
            }
            modify_current_working_dir();
            read_dir(dirname);            
        }
        else if(validate_dirname(dirname, &inode_num)){
            append_to_current_working_dir(dirname);
            read_dir(dirname);
            currentinode = inode_num;
        }
        else{
            fprintf(stderr, "No such Directory!\n");
            return 0;
        }
    }
    return 1;
}
