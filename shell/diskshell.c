/* USP Project - RFileSystem Shell 
    author: rishabh
            rajat
            ravi
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
	/*
	* Shell interaction begins here.
	* Each command is implemented as a callback to the VRFS level functionality.
	* The prompt to the shell is updated by referencing the current working directory
	* stored in the buffer.
	*/
    while(1){
        printf("rfs%s> ", dirpath);
        fflush(stdout);

        if(!fgets(line,sizeof(line),stdin)) break;

        if(line[0]=='\n') continue;
        line[strlen(line)-1] = 0;
        args = sscanf(line,"%s %s %[^\n]",cmd,arg1,arg2);
		if(args==0) continue;

		//format command
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
		} 
		// cd command
		else if(!strcmp(cmd,"cd")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("mount failed! No disk found!\n");
				continue;
			}
			if(args==2) {
				if(!cd(arg1)) {
					printf("No Such Directory to move!\n");
				}
			} else {
				printf("use: cd <dirname>\n");
			}
		} 
		//unmount command
		else if(!strcmp(cmd,"unmount")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("unmount failed! No disk found!\n");
				continue;
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
		} 
		//debug command
		else if(!strcmp(cmd,"debug")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("debug failed! No disk found!\n");
				continue;
			}
			if(args==1) {
				rfs_debug();
			} else {
				printf("use: debug\n");
			}
		} 
		//ls command
		else if(!strcmp(cmd,"ls")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("ls failed! No disk found!\n");
				continue;
			}
			if(args==1) {
				list(argv[1]);
			} else {
				printf("use: ls\n");
			}
		} 
		//touch command
		else if(!strcmp(cmd,"touch")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("touch failed! No disk found!\n");
				continue;
			}
			if(args==2) {
				touch(arg1);
			} else {
				printf("use: touch <file>\n");
			}
		} 
		//mkdir command
		else if(!strcmp(cmd,"mkdir")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("mkdir failed! No disk found!\n");
			}
			if(args==2) {
				strcat(arg1, "\0");
				mkdir(arg1);
			} else {
				printf("use: mkdir <dirname>\n");
			}
		} 
		//rm command
		else if(!strcmp(cmd,"rm")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("rm failed! No disk found!\n");
				continue;
			}
			if(args==2) {
				strcat(arg1, "\0");
				rm(arg1);
			} else {
				printf("use: rm <filename>\n");
			}
		} 
		//rmdir command
		else if(!strcmp(cmd,"rmdir")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("rmdir failed! No disk found!\n");
				continue;
			}
			if(args==2) {
				strcat(arg1, "\0");
				rm_dir(arg1);
			} else {
				printf("use: rmdir <filename>\n");
			}
		} 
		//cat command
		else if(!strcmp(cmd,"cat")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("cat failed! No disk found!\n");
				continue;
			}
			if(args==2) {
				strcat(arg1, "\0");
				read_file(arg1);
			} else {
				printf("use: cat <filename>\n");
			}
		} 
		//echo command
		else if(!strcmp(cmd,"echo")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("echo failed! No disk found!\n");
				continue;
			}
			if(args==3) {
				strcat(arg1, "\0");
				strcat(arg2, "\0");
				write_file(arg1, arg2);
			} else {
				printf("use: echo <data> <filename>\n");
			}
		} 
		//cp command
		else if(!strcmp(cmd,"cp")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("cp failed! No disk found!\n");
				continue;
			}
			if(args==3) {
				strcat(arg1, "\0");
				strcat(arg2, "\0");
				cp_file(arg1, arg2);
			} else {
				printf("use: cp <srcfile> <destfile>\n");
			}
		} 
		//mv command
		else if(!strcmp(cmd,"mv")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("mv failed! No disk found!\n");
				continue;
			}
			if(args==3) {
				strcat(arg1, "\0");
				strcat(arg2, "\0");
				mv(arg1, arg2);
			} else {
				printf("use: mv <oldname> <newname>\n");
			}
		} 
		//mkfs command
		else if(!strcmp(cmd,"mkfs")) {
			if(disk == NULL){
				printf("mkfs failed! No disk found!\n");
				continue;
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
		} 
		//stat command
		else if(!strcmp(cmd,"stat")) {
			if(disk == NULL){
				printf("stat failed! No disk found!\n");
				continue;
			}
			if(args==2) {
				if(!stat_file(arg1)){
                    printf("Error in retrieving the file status!\n");
                }
            } else {
				printf("use: stat <filename>\n");
			}
		} 
		//pwd command
		else if(!strcmp(cmd,"pwd")) {
			if(disk == NULL){
				printf("pwd failed! No disk found!\n");
				continue;
			}
			if(args==1) {
				current_working_dir();
            } else {
				printf("use: pwd\n");
			}
		} 
		//disksize command
		else if(!strcmp(cmd, "disksize")){
			if(disk_mounted == FALSE || disk == NULL){
				printf("disksize failed! No disk found!\n");
				continue;
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
        } 
		//getsize command
		else if(!strcmp(cmd,"getsize")) {
			if(disk_mounted == FALSE || disk == NULL){
				printf("getsize failed! No disk found!\n");
				continue;
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
			
		}
		//cls command
		 else if(!strcmp(cmd,"cls")) {
			if(args == 1){
                system("clear");
            } 
    	} 
		//help command: lists all available commands
		else if(!strcmp(cmd,"help")) {
			printf("Commands are:\n");
			printf("    format\n");
            printf("    unmount\n");
			printf("	cp <src> <dest>");
			printf(" 	mv <oldfilename> <newfilename>");
			printf("    debug\n");
			printf("    cat <file>\n");
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
		}
		// quit the shell
		 else if(!strcmp(cmd,"quit")) {
            printf("Bye\n");
			break;
		} 
		//exit the shell, same as quit
		else if(!strcmp(cmd,"exit")) {
            printf("Bye\n");
			break;
		} 
		//catch all
		else {
			printf("unknown command: %s\n",cmd);
			printf("type 'help' for a list of commands.\n");
			result = 1;
		}
    }
    if(disk != NULL){
		rfs_unmount();
        disk_close();
        printf("disk unmounted.\n");
    }
    return 0;
}
