// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <curl/curl.h>

// struct string {
//   char *ptr;
//   size_t len;
// };

// void init_string(struct string *s) {
//   s->len = 0;
//   s->ptr = malloc(s->len+1);
//   if (s->ptr == NULL) {
//     fprintf(stderr, "malloc() failed\n");
//     exit(EXIT_FAILURE);
//   }
//   s->ptr[0] = '\0';
// }

// size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
// {
//   size_t new_len = s->len + size*nmemb;
//   s->ptr = realloc(s->ptr, new_len+1);
//   if (s->ptr == NULL) {
//     fprintf(stderr, "realloc() failed\n");
//     exit(0);
//   }
//   memcpy(s->ptr+s->len, ptr, size*nmemb);
//   s->ptr[new_len] = '\0';
//   s->len = new_len;

//   return size*nmemb;
// }
// int main(void){
//   CURL *curl;
//   CURLcode res = CURLE_OK;
//   curl = curl_easy_init(); //it is not thread safe, so better call curl_easy_cleanup

//   if(curl) { 
//     curl_easy_setopt(curl, CURLOPT_USERNAME, "2019csb1071@gmail.com");
//     curl_easy_setopt(curl, CURLOPT_PASSWORD, "assignment5");
//     curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/inbox");
//     // curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "SELECT INBOX");
//     // curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "FETCH 3 BODY[TEXT]");
//     // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
//     // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
//     // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//     res = curl_easy_perform(curl);
  
//   struct string s;
//   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
//   curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
//   printf("%s\n", s.ptr);
  
//     if(res != CURLE_OK)
//       fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
//     curl_easy_cleanup(curl);
//   }
//   return (int)res;
// }.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <curl/curl.h>
#include "string/string.h"
#include "curl/curl_.h"

CURL *curl;
CURLcode res;
struct MemoryStruct chunk;

void curl_init(){
    chunk.memory = malloc(1); 
    chunk.size = 0;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_USERNAME, "2019csb1071@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "assignment5");
    curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/filesystem");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
}

char* get_subject(){
    char* uid = "3";
    char* base = "imaps://imap.gmail.com:993/filesystem";
    fetch_subject(base, uid, curl);
    char* parsedSubject = parse_subject(chunk.memory);
    return parsedSubject;
}

void get_files(){
    char* base = "imaps://imap.gmail.com:993/filesystem";
    fetch_files(base, curl);
}

int main(){
    curl_init();
    char* directory = get_directory();
    
    printf("%s\n", directory);
    return 0;
}