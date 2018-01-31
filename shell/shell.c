/* USP Project - RFileSystem Shell 
    author: rishabh
            rajat
            ravi
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands/commands.h"
#include "../memdisk/memdisk.h"
#include "../rfs/rfs.h"
#include "../vrfs/vrfs.h"
#define MAX_LIMIT 1024

int main(int argc, char *argv[]){
    char line[MAX_LIMIT];
    char cmd[MAX_LIMIT];
	char arg1[MAX_LIMIT];
	char arg2[MAX_LIMIT];
    char dirpath[MAX_LIMIT];
	int inumber, result, args;
    dirpath[0] = ' ';

    if(argc != 2){
        printf("use: %s <username>\n", argv[0]);
        return -1;
    }
    printf("Welcome, %s to the RFileSystem Shell. \n", argv[1]);
    disk_init();
    printf("opened in-memory emulated disk image.\n");
    while(1){
        printf("rfs%s> ", dirpath);
        fflush(stdout);

        if(!fgets(line,sizeof(line),stdin)) break;

        if(line[0]=='\n') continue;
        line[strlen(line)-1] = 0;
        args = sscanf(line,"%s %s %s",cmd,arg1,arg2);
		if(args==0) continue;

		if(!strcmp(cmd,"format")) {
			if(args==1) {
				if(rfs_format()) {
					printf("disk formatted.\n");
				} else {
					printf("format failed!\n");
				}
			} else {
				printf("use: format\n");
			}
		} else if(!strcmp(cmd,"mount")) {
			if(args==1) {
				if(make_rfs(argv[1])) {
					printf("disk mounted.\n");
				} else {
					printf("mount failed!\n");
				}
			} else {
				printf("use: mount\n");
			}
		} else if(!strcmp(cmd,"unmount")) {
			if(args==1) {
				if(rfs_unmount()) {
					printf("disk unmounted.\n");
                    printf("Bye\n");
                    break;
				} else {
					printf("unmount failed!\n");
				}
			} else {
				printf("use: unmount\n");
			}
		} else if(!strcmp(cmd,"debug")) {
			if(args==1) {
				rfs_debug();
			} else {
				printf("use: debug\n");
			}
		} else if(!strcmp(cmd,"mkfs")) {
			if(args==1) {
				if(!make_rfs(argv[1])){
                    printf("Error in making the RFileSystem!\n");
                }
                else{
                    strcat(dirpath, argv[1]);
                    strcat(dirpath, "/");
                    printf("RFileSystem has been made and mounted on disk!\n");
                }
            } else {
				printf("use: mkfs\n");
			}
		} else if(!strcmp(cmd, "disksize")){
            if(args == 1){
                result = disk_size();
                if(result >= 0){
                    printf("Disk Remaining Size is: %d bytes\n",
                     ((NUM_BLOCKS)-result)*BLK_SIZE);
                }
                else{
                    printf("Disk size command failed!");
                }
            }
            else{
                printf("use: disksize\n");
            }
        } else if(!strcmp(cmd,"getsize")) {
			if(args==2) {
				inumber = atoi(arg1);
				result = rfs_getsize(inumber);
				if(result>=0) {
					printf("inode %d has size %d\n",inumber,result);
				} else {
					printf("getsize failed!\n");
				}
			} else {
				printf("use: getsize <inumber>\n");
			}
			
		} else if(!strcmp(cmd,"clear")) {
			if(args == 1){
                system("clear");
            } 
    	} else if(!strcmp(cmd,"help")) {
			printf("Commands are:\n");
			printf("    format\n");
			printf("    mount\n");
            printf("    unmount\n");
			printf("    debug\n");
			printf("    cat     <file>\n");
			printf("    ls <dir>\n");
			printf("    pwd\n");
            printf("    mkdir <dirname>\n");
            printf("    rmdir <dirname>\n");
            printf("    rm <file>\n");
            printf("    mkfs\n");
            printf("    touch <file>\n");
            printf("    cd <dir>\n");
            printf("    clear\n");
			printf("    help\n");
			printf("    quit\n");
			printf("    exit\n");
		} else if(!strcmp(cmd,"quit")) {
            printf("Bye\n");
			break;
		} else if(!strcmp(cmd,"exit")) {
            printf("Bye\n");
			break;
		} else {
			printf("unknown command: %s\n",cmd);
			printf("type 'help' for a list of commands.\n");
			result = 1;
		}
    }
    if(disk != NULL){
        rfs_unmount();
        printf("disk unmounted.\n");
    }
    return 0;
}
