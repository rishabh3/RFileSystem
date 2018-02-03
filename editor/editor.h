/*
    FileSystem USP Project:
    author: rishabh
            ravi
            rajat
*/

#ifndef EDITOR_H_
#define EDITOR_H_
#define SIZE (1 << 12 - sizeof(int))

char *filename;
int inode_num;

struct buffer{
    char buffer_area[SIZE];
};

void save();
void open();

#endif