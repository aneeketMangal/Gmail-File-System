

void fetch_subject(char* base, char*uid, CURL* curl){
    char url[strlen(base)+256];
    strcpy(url,base);
    strcat(url,";UID=");
    strcat(url, uid);
    strcat(url, ";SECTION=HEADER.FIELDS%20(Subject)");
    curl_easy_setopt(curl, CURLOPT_URL,url);
    curl_easy_perform(curl);
}

char* parse_subject(char * rawSubject){
    char * parsedSubject = substr(rawSubject, 9, strlen(rawSubject));
    parsedSubject = trim(parsedSubject);
    return parsedSubject;
}

char* getSubject(char* base, char* uid, CURL* curl, struct MemoryStruct chunk){
    fetch_subject(base, uid, curl);
    char* parsedSubject = parse_subject(chunk.memory);
    return parsedSubject;
}           