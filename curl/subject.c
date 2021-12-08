

char* fetch_subject(char* base, char*directory, char*uid){
    CURL * curl = curl_init();
    chunk_init;
    char url[strlen(base)+256];
    strcpy(url,directory);
    strcat(url, base);
    strcat(url,";UID=");
    strcat(url, uid);
    strcat(url, ";SECTION=HEADER.FIELDS%20(Subject)");
    curl_easy_setopt(curl, CURLOPT_URL,url);
    curl_easy_perform(curl);
    char* rawSubject = chunk.memory;
    curl_easy_cleanup(curl);
    return rawSubject;
}

char* parse_subject(char * rawSubject){
    char * parsedSubject = substr(rawSubject, 9, strlen(rawSubject));
    parsedSubject = trim(parsedSubject);
    return parsedSubject;
}

char* getSubject(char* base, char* directory, char* uid){
    char* rawSubject = fetch_subject(directory, base, uid);
    char* parsedSubject = parse_subject(rawSubject);
    free(rawSubject);
    return parsedSubject;
}           