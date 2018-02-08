/* USP File System Project RFileSystem 
    author: rishabh 
            ravi
            rajat
*/

#ifndef COMMANDS_H_
#define COMMANDS_H_
#define NUM_BLOCK_PER_FILE 1 << 2
int list(char* path);
int touch(char* file);
int stat_file(char* file);
void current_working_dir();
int mkdir(char* dirname);
#endif