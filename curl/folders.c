

void fetch_folders(char* base, char* directory, CURL* curl){
    char url[strlen(base)+256];
    strcpy(url,base);
    strcat(url, directory);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_perform(curl);
}

char** parse_folders(char* base, char* directory, int* folderCount){
    CURL * curl = curl_init();
    chunk_init;
    fetch_folders(base, directory, curl);
    if(chunk.size ==0){
        return NULL;
    }
    char * rawData = chunk.memory;
    printf("%s\n", rawData);
    char token[256] = "LIST (\\HasNoChildren) \"/\" ";
    strcat(token, "\"filesystem");
    strcat(token, directory);
    strcat(token, "/");


    char token2[256] = "LIST (\\HasChildren) \"/\" ";
    strcat(token2, "\"filesystem");
    strcat(token2, directory);
    strcat(token2, "/");
    char **result = NULL;
    int count = 0;
    char *pch;

    // split
    pch = strtok (rawData,"*");
    while (pch != NULL)
    {
        if (strstr(pch, token) != NULL)
        {
            char* parsed = pch +strlen(token)+1;
            parsed[strlen(parsed)-3] = '\0'; // remove the last character
            if(strnst(parsed, '/') == 0){
                result = (char**)realloc(result, sizeof(char*)*(count+1));
                result[count] = (char*)malloc(strlen(parsed)+1);
                strcpy(result[count], parsed);
                count++;
            }
        }
        else if (strstr(pch, token2) != NULL){
            char* parsed = pch +strlen(token2)+1;
            parsed[strlen(parsed)-3] = '\0'; // remove the last character
            if(strnst(parsed, '/') == 0){
                result = (char**)realloc(result, sizeof(char*)*(count+1));
                result[count] = (char*)malloc(strlen(parsed)+1);
                strcpy(result[count], parsed);
                count++;
            }
        }
        pch = strtok (NULL, "*");
    }
    
    *folderCount = count;
    printf("%d\n", count);
    printf("folders: %s\n", directory);
    curl_easy_cleanup(curl);
    return result;
    // free(curl); 
}