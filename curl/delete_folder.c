/*
    Utility to delete the folders/directories
*/
int delete_folder(char* base, char* path){
    int * fileCount = malloc(sizeof(int));
	*fileCount = 0;
    int * folderCount = malloc(sizeof(int));
    *folderCount = 0;
    printf("files done: %s\n", path);
	char ** files = parse_files(base, path, fileCount);
    printf("files done: %s\n", path);
    printf("files done\n");
	char ** folders = parse_folders(base, path, folderCount);
    printf("files done\n");
    int deletAble = 0;
	if(*fileCount == 0 && *folderCount == 0){ // no files or folders in the given path
       deletAble = 1;
    }		
    for(int i = 0;i<*fileCount;i++){
        free(files[i]);
    }
    for(int i = 0;i<*folderCount;i++){
        free(folders[i]);
    }
    	
    if(deletAble){
        CURL * curl = curl_init();
        chunk_init;
        char request[512];
        strcpy(request, "DELETE filesystem");
        strcat(request, path);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, base);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        chunk_free;
        return 0;
    }
    return -1;
}

