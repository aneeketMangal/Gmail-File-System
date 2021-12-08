/*
    Utility to delete files.
    
*/
int delete_file(char* base, char* path){
    char * fileName = strrchr(path, '/'); // fetching the file name
    fileName++; // skipping the '/'
    char * directory = malloc(sizeof(char)*(strlen(path)-strlen(fileName))); // fetching the directory
    strncpy(directory, path, strlen(path)-strlen(fileName)-1);
    directory[strlen(path)-strlen(fileName)-1] = '\0';

    char* uid = fetch_uid(base, directory, fileName);
    uid = substr(uid, strlen("* SEARCH "), strlen(uid));

    trim(uid);
    // free(directory);
    if(strlen(uid) == 0 || isspace(uid[0])){
        free(uid);
        return 0;
    }

    CURL * curl = curl_init();
    chunk_init;


    char request[512];
    strcpy(request, "UID STORE ");
    strcat(request, uid);
    strcat(request, " +Flags \\Deleted");
    printf("request: %s\n", request);
    char url[512];
    strcpy(url, base);
    strcat(url, directory);

    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    printf("delete url: %s\n",url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request);
    curl_easy_perform(curl);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "EXPUNGE");
    printf("expunge: %s\n",url);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    free(directory);
    free(uid);
    chunk_free;
    return 1;
}

