/* USP Project - RFileSystem Shell 
    author: rishabh
            rajat
            ravi
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands/commands.h"
#include "../disk/disk.h"
#include "../rfsdisk/rfsdisk.h"
#include "../vrfs/vrfs.h"
#define MAX_LIMIT 1024

int main(int argc, char *argv[]){
    char line[MAX_LIMIT];
    char cmd[MAX_LIMIT];
	char arg1[MAX_LIMIT];
	char arg2[MAX_LIMIT];
    char dirpath[MAX_LIMIT];
    int count = 0;
	int inumber, result, args;
    dirpath[0] = ' ';

    if(argc != 2){
        printf("use: %s <username>\n", argv[0]);
        return -1;
    }
    printf("Welcome, %s to the RFileSystem Shell. \n", argv[1]);
    disk_init("diskfile", "r+");
    if(disk == NULL){
        return -1;
    }
    printf("opened file emulated disk image.\n");
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
			if(disk_mounted == FALSE || disk == NULL){
				printf("mount failed! No disk found!\n");
			}
			if(args==1) {
				if(make_rfs(argv[1])) {
					printf("disk mounted.\n");
				} else {
					printf("mount failed!\n");
				}
			} else {
				printf("use: mount\n");
			}
		} else if(!strcmp(cmd,"cd")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("mount failed! No disk found!\n");
			}
			if(args==2) {
				if(!cd(arg1)) {
					printf("No Such Directory to move!\n");
				}
			} else {
				printf("use: cd <dirname>\n");
			}
		} else if(!strcmp(cmd,"unmount")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("unmount failed! No disk found!\n");
			}
			if(args==1) {
				if(rfs_unmount()) {
					memset(dirpath, '\0', MAX_LIMIT);
					dirpath[0] = ' ';
					count = 0;
					printf("disk unmounted.\n");
				} else {
					printf("unmount failed!\n");
				}
			} else {
				printf("use: unmount\n");
			}
		} else if(!strcmp(cmd,"debug")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("debug failed! No disk found!\n");
			}
			if(args==1) {
				rfs_debug();
			} else {
				printf("use: debug\n");
			}
		} else if(!strcmp(cmd,"ls")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("ls failed! No disk found!\n");
			}
			if(args==1) {
				list(argv[1]);
			} else {
				printf("use: ls\n");
			}
		} else if(!strcmp(cmd,"touch")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("touch failed! No disk found!\n");
			}
			if(args==2) {
				touch(arg1);
			} else {
				printf("use: touch <file>\n");
			}
		} else if(!strcmp(cmd,"mkdir")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("mkdir failed! No disk found!\n");
			}
			if(args==2) {
				strcat(arg1, "\0");
				mkdir(arg1);
			} else {
				printf("use: mkdir <dirname>\n");
			}
		} else if(!strcmp(cmd,"mkfs")) {
			if(disk == NULL){
				printf("mkfs failed! No disk found!\n");
			}
			if(args==1) {
				if(!make_rfs(argv[1])){
                    printf("Error in making the RFileSystem!\n");
                }
                else{
                    if(count == 0){
                        strcat(dirpath, argv[1]);
                        strcat(dirpath, "/");
                    }
                    printf("RFileSystem has been made and mounted on disk!\n");
                }
            } else {
				printf("use: mkfs\n");
			}
            count++;
		} else if(!strcmp(cmd,"stat")) {
			if(disk == NULL){
				printf("stat failed! No disk found!\n");
			}
			if(args==2) {
				if(!stat_file(arg1)){
                    printf("Error in retrieving the file status!\n");
                }
            } else {
				printf("use: stat <filename>\n");
			}
		} else if(!strcmp(cmd,"pwd")) {
			if(disk == NULL){
				printf("stat failed! No disk found!\n");
			}
			if(args==1) {
				current_working_dir();
            } else {
				printf("use: pwd\n");
			}
		} else if(!strcmp(cmd, "disksize")){
			if(disk_mounted == FALSE || disk == NULL){
				printf("disksize failed! No disk found!\n");
			}
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
			if(disk_mounted == FALSE || disk == NULL){
				printf("getsize failed! No disk found!\n");
			}
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
			
		} else if(!strcmp(cmd,"cls")) {
			if(args == 1){
                system("cls");
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
        disk_close();
        printf("disk unmounted.\n");
    }
    return 0;
}
