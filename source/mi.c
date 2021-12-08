
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
#include "string/string.h"
#include "curl/curl_.h"
#include "constants.c"


char dir_list[MAX_DIR_COUNT][DIR_NAME];
int curr_dir_idx = -1;

char files_list[MAX_FILE_COUNT][FILE_NAME];
int curr_file_idx = -1;

char files_content[ 256 ][ 256 ];
int curr_file_content_idx = -1;

CURL *curl; 
CURLcode res; // to store the response of some curl_easy_perform()

struct MemoryStruct chunk;
void add_dir(const char *dir_name)
{
	curr_dir_idx++;
	strcpy(dir_list[ curr_dir_idx ], dir_name);
	char b[FILE_NAME+10] = "CREATE filesystem/";
	char a[FILE_NAME+10];
	strcpy(a, dir_name);
	strcat(b, a);
	/*
		using curl to create a corresponding directory on the server, 
	*/
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, b); 

	res = curl_easy_perform(curl);
	if(res != CURLE_OK) // in case the label can not be created
	{
		printf("curl failed: %s\n", curl_easy_strerror(res));
	}
}

int isDir(const char *path)
{
	return is_dir("filesystem", path, curl);
}

void add_file( const char *filename )
{
	curr_file_idx++;
	strcpy( files_list[ curr_file_idx ], filename );
	
	curr_file_content_idx++;
	strcpy( files_content[ curr_file_content_idx ], "" );
}

int is_file( const char *path )
{
	path++; // Eliminating "/" in the path
	
	for ( int curr_idx = 0; curr_idx <= curr_file_idx; curr_idx++ )
		if ( strcmp( path, files_list[ curr_idx ] ) == 0 )
			return 1;
	
	return 0;
}

int get_file_index( const char *path )
{
	path++; // Eliminating "/" in the path
	
	for ( int curr_idx = 0; curr_idx <= curr_file_idx; curr_idx++ )
		if ( strcmp( path, files_list[ curr_idx ] ) == 0 )
			return curr_idx;
	
	return -1;
}

void write_to_file( const char *path, const char *new_content )
{
	int file_idx = get_file_index( path );
	
	if ( file_idx == -1 ) // No such file
		return;
		
	strcpy( files_content[ file_idx ], new_content ); 
}

// ... //

static int do_getattr( const char *path, struct stat *st )
{
	st->st_uid = getuid(); // The owner of the file/directory is the user who mounted the filesystem
	st->st_gid = getgid(); // The group of the file/directory is the same as the group of the user who mounted the filesystem
	st->st_atime = time( NULL ); // The last "a"ccess of the file/directory is right now
	st->st_mtime = time( NULL ); // The last "m"odification of the file/directory is right now
	
	if (strcmp(path, "/" ) == 0 || isDir( path ) == 1)
	{
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 2; // Why "two" hardlinks instead of "one"? The answer is here: http://unix.stackexchange.com/a/101536
	}
	else if (is_file( path ) == 1)
	{
		st->st_mode = S_IFREG | 0644;
		st->st_nlink = 1;
		st->st_size = 1024;
	}
	else
	{
		return -ENOENT;
	}
	
	return 0;
}

static int do_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi )
{
	filler( buffer, ".", NULL, 0, 0); // Current Directory
	filler( buffer, "..", NULL, 0, 0); // Parent Directory
	
	if ( strcmp( path, "/" ) == 0 ) // If the user is trying to show the files/directories of the root directory show the following
	{

		fetch_files(IMAP_BASE_PATH, "", curl);
		int fileCount = count_files(chunk.memory);
		char ** files = parse_files(chunk.memory, fileCount);
		for(int i = 0;i<fileCount;i++){
			filler( buffer, files[i], NULL, 0, 0);
			// printf("%s\n", files[i]); 
		}
		free(files);
		// get_dir_list( "filesystem", dir_list, curl );
		// get_file_list( "filesystem", files_list, curl );
		// for ( int curr_idx = 0; curr_idx <= curr_dir_idx; curr_idx++ )
		// 	filler( buffer, dir_list[ curr_idx ], NULL, 0, 0);
	
		// for ( int curr_idx = 0; curr_idx <= curr_file_idx; curr_idx++ )
		// 	filler( buffer, files_list[ curr_idx ], NULL, 0, 0);
	}
	
	return 0;
}

static int do_read( const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi )
{
	int file_idx = get_file_index( path );
	
	if ( file_idx == -1 )
		return -1;
	
	char *content = files_content[ file_idx ];
	
	memcpy( buffer, content + offset, size );
		
	return strlen( content ) - offset;
}


/*
	function for modifying the mkdir command
	NOTE: 
	* the path is relative to the root directory
	* only single-layer directory creation is supported
	* the directory name should not contain "/"
*/
static int do_mkdir(const char *path, mode_t mode)
{	
	path++; // Eliminating "/" in the path
	int isNested = 0;
	for(int i = 0;i<strlen(path);i++)
	{
		if(path[i]=='/')
		{
			isNested = 1;
			break;
		}
	}
	if(isNested == 0){
		add_dir(path);
	}
	else{
		printf("Support for nested paths has not been included yet\n");
		return -ENOENT;
	}
	return 0;
}

static int do_mknod( const char *path, mode_t mode, dev_t rdev )
{
	path++;
	add_file( path );
	
	return 0;
}

static int do_write( const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info )
{
	write_to_file(path, buffer);
	return size;
}

static struct fuse_operations operations = {
    .getattr	= do_getattr,
    .readdir	= do_readdir,
    .read		= do_read,
    .mkdir		= do_mkdir,
    .mknod		= do_mknod,
    .write		= do_write,
};

int main( int argc, char *argv[] )
{
	chunk.memory = malloc(1); 
    chunk.size = 0;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_USERNAME, "2019csb1071@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "assignment5");
    curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/filesystem");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	return fuse_main( argc, argv, &operations, NULL );
}