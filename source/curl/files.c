

void fetch_files(char* base, char* directory, CURL* curl){
    char url[strlen(base)+256];
    strcpy(url,base);
    strcat(url, directory);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "UID SEARCH ALL");
    curl_easy_perform(curl);

}

int count_files(char * rawSubject){
    char * parsedSubject = substr(rawSubject, strlen("* SEARCH "), strlen(rawSubject));
    trim(parsedSubject);
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


char** parse_files(char * rawSubject, int fileCount){
    char * parsedSubject = substr(rawSubject, strlen("* SEARCH "), strlen(rawSubject));
    trim(parsedSubject);
    char *ptr = strtok(parsedSubject, " "); // splitting the string using space delimiter
    char ** files= (char**)malloc(fileCount * sizeof(char*));
	int i = 0;
    while(ptr != NULL)
	{
        ptr = trim(ptr);
        // files[i] = (char*)malloc(strlen(ptr) * sizeof(char));       
        files[i] = ptr;
        i+=1;
		ptr = strtok(NULL, " ");
	}
    return files; 
}