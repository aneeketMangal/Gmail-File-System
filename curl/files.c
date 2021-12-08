

char* fetch_files(char* base, char* directory){
    CURL * curl = curl_init();
    chunk_init;
    char url[strlen(base)+256];
    strcpy(url,base);
    strcat(url, directory);
    printf("url: %s\n", url);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "UID SEARCH ALL");
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    printf("chunk: %s\n", chunk.memory);
    return chunk.memory;
}

int count_files(char * rawSubject){
    char * parsedSubject = substr(rawSubject, strlen("* SEARCH "), strlen(rawSubject));
    trim(parsedSubject);
    if(strcmp(parsedSubject, "\n") == 0){
        return 0;
    }

    int fileCount = 0;
	char *ptr = strtok(parsedSubject, " "); // splitting the string using space delimiter
    while(ptr != NULL)
	{
        ptr = trim(ptr);
		ptr = strtok(NULL, " ");
        fileCount+=1;
	}
    return fileCount;
}


char** parse_files(char* base, char* directory, int* fileCount){
   
    char* rawFiles = fetch_files(base, directory);
    *fileCount = count_files(rawFiles);
    if(*fileCount == 0){
        return NULL;
    }

    char * parsedSubject = substr(rawFiles, strlen("* SEARCH "), strlen(rawFiles));
    trim(parsedSubject);
    char *ptr = strtok(parsedSubject, " "); // splitting the string using space delimiter
    char ** files= (char**)malloc(*fileCount * sizeof(char*));
	int i = 0;
    while(ptr != NULL)
	{
        ptr = trim(ptr);

        char * file = getSubject(base, directory, ptr);
        files[i] = (char*)malloc(strlen(file));
        strcpy(files[i], file);
        files[i] = file;
        i+=1;
		ptr = strtok(NULL, " ");
	}
    free(rawFiles);
    return files;
}