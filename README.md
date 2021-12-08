<center><h1>Gmail FS</h1></center>

This program is based on FUSE system that enables to use email account as a disk. Few details:
* File name is the subject of the email.
* Body corresponds to the body content itself.
* Only non-binary data is present in the file.
* Commands like ls, cd, mkdir, cat, rm, rmdir etc would work as expected.

## Description
* This program mounts a local library to a email server, here in this program's case, the server is IMAP.
* To change and modify the functions, libfuse library has been used.
* A libfuse based filesystem has been created, and following functions have been implemented
    * gmailFS_mkdir: the corresponding function which is called when mkdir function is called.
    * gmailFS_getattr: This function is used to get the attributes of a file/directory(permissons, maximum size, choice if if the entity is file or a directory).
    * gmailFS_rmdir: removes a directory.
    * gmailFS_unlink: removes a file.
    * gmailFS_read: This function read the content of files
    * gmailFS_readdir: This function 
    * gmailFS_mknod: This functions creates a new file(corresponding to a new email).
    * gmailFS_write: This function writes to the file, to the corresponding email

* The corresponding functions for recieving/sending data 
from the IMAP server, have been implemented using libcurl.
These functions can be found in the curl folder in source
 
* Various functionalities of libcurl have been used:
    * fetching email body from the server
    * fetching files from the server
    * sending email
    * checking if a path is valid
    * list of directories present
    * delete folder
    * delete files
* IMAP responses are in a fixed format, which are not convenient to parse into program, so there is a lot of string processing involved. For this some string.h library along with some manually created functions have been used.
The implementation of the string helper functions can be found in the string folder.
* The implementation is based on the following format, a extra label has been created which is the root label, the entire filesystem is inside this label.
* This label's name is filesystem

## Instructions to compile & run
* INSTALL following in the system:
    * Libfuse
    * Libcurl
    * pkg-config

* The given program is meant espacially for the gmail, all the testing etc. have been done using that.
* "Compulsory:" Create a label filesystem on your gmail.
* Decide a mount folder, and it should be inside /home/user directory
* Run "fusermount -u directory"
* Navigate to ```source``` directory inside main directory.
* To edit config file (config.h),  edit the defined macros. **Compuslory**
* Enter the complete email and not just the username.
* Ensure that the credentials are correct
* Since the data is being fetched from a remote server via internet, the responses would slow and take time.
* The default IMAP server URL is already present in the file.
* Compile using ```gcc -Wall main.c `pkg-config fuse3 --cflags --libs` -lcurl -o main```.
* Run using  ```./main -f <mount directory> <config file directory>```
* Then you can run the ls cd etc commands on a separate terminal once you have located to the mount point 

