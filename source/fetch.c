#include <stdio.h>
#include <curl/curl.h>

int main(void){
   CURL *curl;
   CURLcode res = CURLE_OK;
   curl = curl_easy_init(); //it is not thread safe, so better call curl_easy_cleanup

   if(curl) { 
     curl_easy_setopt(curl, CURLOPT_USERNAME, "2019csb1071@iitrpr.ac.in");
     curl_easy_setopt(curl, CURLOPT_PASSWORD, "207kavi4067");
     curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/");
     curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
     curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    //  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
     res = curl_easy_perform(curl);
     if(res != CURLE_OK)
       fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
     curl_easy_cleanup(curl);
   }
return (int)res;
}