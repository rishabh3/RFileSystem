# The RFileSystem
## A simple filesystem. Comes with it's own shell.
## Project for the Unix Systems Programming Lab(UE15CS355)

### Instructions to run
Clone the repository. Make a directory named **bin** in project root folder. Run the following commands from inside the root of the project:
```
make
./rdiskshell <user_name>
```
This launches the shell for interfacing with our filesystem. Before you can start using our file system, you will have to format the simulated disk(if you are accessing it for the first time) and then mount the file system.
```
format
mkfs
```
```
help
```
Lists the commands that you can run from within our shell. 

Use
```
make clean
```
To clean up after the project.

## Project Description
The RFileSystem is a simple file system arranged in layers and designed to be quite modular.

### Memdisk and Disk
At the bottom layer of our file system, we have our memdisk and our disk directories.  
Memdisk is used to interface with our simulated disk in memory and disk is used to interface with our simulated persistent disk implemented as a file. These modules are designed to mimic 
device drivers since they are responsible for actually writing and reading blocks of data(4096B) to and from our simulated disk, whether in memory or persistent. 

### RFS
The next layer in the hierarchy is the RFS level. This is the level where the actual file system is implemented. RFS interfaces with the simulated disk using either memdisk or disk and writes the file system onto the simulated disk. This means writing the super block as well as allocating and initialing inodes. At the RFS level everything is a file.

Our super block stores metadata about the total number of blocks on disk, total number of inode and data blocks as well as the number of blocks in use. 

10% of the total number of blocks have been set aside as inode blocks and each inode block can hold 48 inodes. Each inode has four direct pointers to data blocks. This sets a hard limit on the size of our files to 16KB. 

RFS supports a few functions. These include:
* rfs_debug(): Gets information about the file system.
* rfs_format(): Formats the simulated disk for use.
* rfs_mount(): Mounts the simulated disk if it has a valid filesystem on it.
* rfs_read(): Reads from a file.
* rfs_write(): Writes to a file.
* rfs_delete(): Deletes a file.
* rfs_getattr(): Returns metadata about the file.

### VRFS
The upper layer in our hierarchy, the VRFS level, serves as a layer of abstraction to the underlying RFS. VRFS employs the concepts of directories, the actual file system tree and current working directories. VRFS supports the same functionality in terms of reading, writing, deletion and so on. The difference here is that these operations are extended to work on directories and essentially call the underlying RFS functions.

### RShell
A simple interactive shell that performs a series of callbacks to the VRFS level to implement the shell commands.

The source code has been reasonably commented so feel free to give it a glance.

### Contributors
* Rishabh Dutt
* Ravi Shreyas Anupindi
* Rajat Nigam
