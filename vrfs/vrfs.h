/*
 * vrfs.h
 *
 *  Created on: 28-Jan-2018
 *      Author: rishabh
 */

#ifndef VRFS_VRFS_H_
#define VRFS_VRFS_H_
#include "../util.h"
#define MAX_SIZE 31

struct dentry;


struct dentry{
    char name[MAX_SIZE];
    int inode_num;
};

/* File Operations */
int touch(char* filename);
int open(char *filename);
int close(int filedesc);
bool read(int filedesc, char *data, rsize_t size);
bool write(int filedesc, char *data, rsize_t size);
roff_t lseek(int filedesc, roff_t offset); 
int rm_file(char* filename);

/*Directory Operations*/
char* present_working_directory();
int make_directory(char *dirname);
int remove_directory(char* dirname);
struct dentry * read_dir(char *dirname);
int change_directory(char *dirname);

#endif /* VRFS_VRFS_H_ */
