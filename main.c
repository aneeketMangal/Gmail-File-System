#define FUSE_USE_VERSION 30
#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <curl/curl.h>
#include <ctype.h>
#include "config.h"
#include "const.h"
#include "string/string.h"
#include "curl/curl_.h"


int isDir(const char *path)
{
	char* temp = (char*)malloc(sizeof(path));
	strcpy(temp, path); 
	int fd = is_dir(ROOT, temp);
	printf("isDir: %s %d\n", temp, fd);
	free(temp);	
	return fd;
}

int is_file(const char *path)
{
	char* temp = (char*)malloc(sizeof(path));
	strcpy(temp, path); 
	printf("path: %s\n", temp);
	int fd = isFile(IMAP_BASE_PATH, temp);
	printf("isFile: %s %d\n", temp, fd);
	free(temp);	
	return fd;
}

static int gmailFS_getattr( const char *path, struct stat *st, struct fuse_file_info * info)
{
	st->st_uid = getuid(); 
	st->st_gid = getgid();
	st->st_atime = time(NULL);
	st->st_mtime = time(NULL);
	
	printf("getarr: %s\n", path);
	if (strcmp(path, "/" ) == 0 || isDir(path) == 1)
	{
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 2; 
	}
	else if (is_file(path) == 1)
	{
		st->st_mode = S_IFREG | 0644;
		st->st_nlink = 1;
		st->st_size = 4096;
	}
	else
		return -ENOENT;
	
	return 0;
}

static int gmailFS_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags )
{
	filler( buffer, ".", NULL, 0, 0); // Current Directory
	filler( buffer, "..", NULL, 0, 0); // Parent Directory
	
	if ( strcmp( path, "/" ) == 0 ) // If the user is trying to show the files/directories of the root directory show the following
	{
		int * fileCount = malloc(sizeof(int));
		*fileCount = 0;
		char ** files = parse_files(IMAP_BASE_PATH, "",fileCount);
		for(int i = 0;i<*fileCount;i++){
			filler( buffer, files[i], NULL, 0, 0);
		}
		
		int * folderCount = malloc(sizeof(int));
		*folderCount = 0;
		char ** folders = parse_folders(IMAP_BASE_PATH, "",folderCount);
		for(int i = 0;i<*folderCount;i++){
			filler(buffer, folders[i], NULL, 0, 0);
		}
		
		for(int i = 0;i<*fileCount;i++){
			free(files[i]);
		}
		for(int i = 0;i<*folderCount;i++){
			free(folders[i]);
		}
		if(files) free(files);
		if(folders) free(folders);
	}
	else if(strlen(path)>0 && path[0] == '/'){ //paths other than root directory
		char* path2 = malloc(strlen(path));
		strcpy(path2, path);
		int * fileCount = malloc(sizeof(int));
		*fileCount = 0;
		char ** files = parse_files(IMAP_BASE_PATH, path2,fileCount);
		for(int i = 0;i<*fileCount;i++){
			printf("files: %s\n", files[i]);
			filler( buffer, files[i], NULL, 0, 0);
		}
		
		int * folderCount = malloc(sizeof(int));
		*folderCount = 0;
		char ** folders = parse_folders(IMAP_BASE_PATH, path2,folderCount);
		for(int i = 0;i<*folderCount;i++){
			filler(buffer, folders[i], NULL, 0, 0);
		}
		
		for(int i = 0;i<*fileCount;i++){
			free(files[i]);
		}
		for(int i = 0;i<*folderCount;i++){
			
			free(folders[i]);
		}
		if(files) free(files);
		if(folders) free(folders);
		free(path2);
		printf("path read: %s\n", path);
	}
	return 0;
}

static int gmailFS_read( const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi )
{
	printf("readFS: %s\n", path);
	int isFile = is_file(path);
	
	if (isFile == 0) return -1;

	char* temp = (char*)malloc(sizeof(path));
	strcpy(temp, path); 
    char* message = parse_message(IMAP_BASE_PATH, temp);
	printf("message: %s\n", message);
	memcpy( buffer, message + offset, size );
	free(temp);	
	return strlen(message) - offset;
}


/*
	function for modifying the mkdir command
	NOTE: 
	* the path is relative to the root directory
	* only single-layer directory creation is supported
	* the directory name should not contain "/"
*/
static int gmailFS_mkdir(const char *path, mode_t mode)
{	
	printf("MKDIR: %s\n", path);
	addDir(path);
	return 0;
}

static int gmailFS_unlink(const char *path)
{
	if(is_file(path) == 0) return -1; // if the file does not exist
	char* temp = (char*)malloc(sizeof(path));
	strcpy(temp, path); 
	int fd = delete_file(IMAP_BASE_PATH, temp);
	free(temp);	
	if(fd) return 0;
	return -1;
}

static int gmailFS_rmdir(const char *path)
{
	if(isDir(path) == 0) return -1; // if the directory does not exist
	char* temp = (char*)malloc(sizeof(path));
	strcpy(temp, path); 
	int fd = delete_folder(IMAP_BASE_PATH, temp);
	free(temp);	
	return fd;
}

static int gmailFS_mknod(const char *path, mode_t mode, dev_t rdev)
{
	if(is_file(path)) return -1; // checking if the file already exists
	printf("mknod called\n");
	char* temp = malloc(sizeof(path));
	strcpy(temp, path);
	send_mail(temp, "I am the gmail fs!!");
	free(temp);
	printf("mknod ended\n");
	return 0;
}

static int gmailFS_write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info)
{
	printf("writeFS: %s\n", path);
	char* temp = (char*)malloc(sizeof(path));
	strcpy(temp, path); 
	if(is_file(path)) { // if file exists, delete the existing file first
		delete_file(IMAP_BASE_PATH, temp);
	}
	temp[0] = '\0';
	strcpy(temp, path);
	send_mail(temp, buffer);
	free(temp);
	printf("write ended\n");
	return size;
}

static struct fuse_operations operations = {
    .getattr	= gmailFS_getattr,
    .readdir	= gmailFS_readdir,
    .read		= gmailFS_read,
    .mkdir		= gmailFS_mkdir,
	.mknod      = gmailFS_mknod,
    .write		= gmailFS_write,
	.unlink		= gmailFS_unlink,
	.rmdir		= gmailFS_rmdir,
};

int main(int argc, char *argv[])
{
	if(argc!=4){
		printf("Usage: -f <mountpath> <config file path>");
		return -1;
	}
	return fuse_main(3, argv, &operations, NULL);
}