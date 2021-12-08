

char* fetch_body_by_uid(char* base, char* directory, char*uid){
    CURL * curl = curl_init();
    chunk_init;
    char url[strlen(base)+256];
    strcpy(url,base);
    strcat(url,directory);
    strcat(url,";UID=");
    strcat(url, uid);
    strcat(url, ";SECTION=1");
    curl_easy_setopt(curl, CURLOPT_URL,url);
    curl_easy_perform(curl);

    if(chunk.size == 0){
        return "";
    }
    else return chunk.memory;
}

char * fetch_uid(char* base, char* directory, char*fileName){
    CURL * curl = curl_init();
    chunk_init;
    char url[strlen(base)+256];
    strcpy(url,base);
    strcat(url,directory);
    curl_easy_setopt(curl, CURLOPT_URL,url);
    char request[512];
    strcpy(request, "UID SEARCH HEADER SUBJECT \"");
    strcat(request, fileName);
    strcat(request, "\"");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST,request);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if(chunk.size ==0){
        return "";
    }
    return chunk.memory;
}

int isFile(char* base, char* path){
    if(strstr(path, "/") == NULL){ //checking if the path is invalid/empty
        return 0;
    }
    
    char * fileName = strrchr(path, '/'); // fetching the file name
    fileName++; // skipping the '/'
    char * directory = malloc(sizeof(char)*(strlen(path)-strlen(fileName))); // fetching the directory
    strncpy(directory, path, strlen(path)-strlen(fileName)-1);
    directory[strlen(path)-strlen(fileName)-1] = '\0';
    char* uid = fetch_uid(base, directory, fileName);
    uid = substr(uid, strlen("* SEARCH "), strlen(uid));
    trim(uid);
    free(directory);
    if(strlen(uid) == 0 || isspace(uid[0])){
        free(uid);
        return 0;
    }
    free(uid);
    return 1;
}

/*
    brief:
        This function is used to parse the body of the email.
    param:
        path: the physical path of the email in terms of mounted directory.
        Eg: /directory/filename.eml
        base: the base url of the email server.
*/

char* parse_message(char* base, char* path){
    if(strstr(path, "/") == NULL){ //checking if the path is invalid/empty
        return "";
    }
    
    char * fileName = strrchr(path, '/'); // fetching the file name
    fileName++; // skipping the '/'
    char * directory = malloc(sizeof(char)*(strlen(path)-strlen(fileName))); // fetching the directory
    strncpy(directory, path, strlen(path)-strlen(fileName)-1);
    directory[strlen(path)-strlen(fileName)-1] = '\0';

    char* uid = fetch_uid(base, directory, fileName);
    if(strlen(uid) == 0){
        free(uid);
        return "";
    }
    CURL * curl = curl_init();
    chunk_init;
    uid = substr(uid, strlen("* SEARCH "), strlen(uid));
    trim(uid);
    if(strlen(uid) == 0 || isspace(uid[0])){
        free(uid);
        free(directory);
        return "";
    }
    char* message = fetch_body_by_uid(base, directory, uid);
    free(uid);
    free(directory);
    if(strlen(message) == 0) return "";
    return message;
}
