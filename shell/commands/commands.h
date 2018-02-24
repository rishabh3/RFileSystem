/* USP File System Project RFileSystem 
    author: rishabh 
            ravi
            rajat
*/

#ifndef COMMANDS_H_
#define COMMANDS_H_
#define NUM_BLOCK_PER_FILE 1 << 2
#define DATA_SIZE (NUM_BLOCK_PER_FILE)*(4092)
int list(char * dirname);
int touch(char* file);
int stat_file(char* file);
void current_working_dir();
int mkdir(char* dirname);
int cd(char *dirname);
int rm(char* filename);
int read_file(char* filename);
int write_file(char *filename, char *data);
int rm_dir(char *dirname);
#endif