#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <curl/curl.h>
#include <ctype.h>
#include "string/string.h"
#include "curl/curl_.h"
#include "constants.c"
CURL *curl;
CURLcode res;
struct MemoryStruct chunk;

void curl_init(){
    chunk.memory = malloc(1); 
    chunk.size = 0;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_USERNAME, "2019csb1071@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "assignment5");
    // curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/%5BGmail%5D/Trash");
    curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/%5BGmail%5D/");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);  
}

char* get_subject(){
    char* uid = "1";
    char* base = "imaps://imap.gmail.com:993/filesystem";
    fetch_subject(IMAP_BASE_PATH, uid, curl);
    char* parsedSubject = parse_subject(chunk.memory);
    return parsedSubject;
}

void get_files(){
    // char* base = "imaps://imap.gmail.com:993/inbox";
    fetch_files(IMAP_BASE_PATH, "", curl);
    printf("ui: %s", chunk.memory);
    int fileCount = count_files(chunk.memory);
    char ** files = parse_files(chunk.memory, fileCount);
    for(int i = 0;i<fileCount;i++){
        printf("%s\n", files[i]); 
    }
    free(files);
}



int main(){
    curl_init();
    // int temp = is_path(IMAP_BASE_PATH, "", curl);
    // printf("%d\n", temp);
    // curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "UID SEARCH ALL");
    res = curl_easy_perform(curl);
    // if(res != CURLE_OK) // in case the label can not be created
    // {
    //     printf("curl failed: %s\n", curl_easy_strerror(res));
    // }
    // else{
      printf("%s\n", chunk.memory);
    // }
    // char* directory = get_subject();
    // printf("%s\n", directory);
    // int temp = is_dir(IMAP_BASE_PATH, "", curl);
    // printf("%d\n", temp);
    // get_files();
    return 0;
}