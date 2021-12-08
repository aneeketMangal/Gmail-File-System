#include <stdio.h>
#include <curl/curl.h>
 
/* This is a simple example showing how to create a new mailbox folder using
 * libcurl's IMAP capabilities.
 *
 * Note that this example requires libcurl 7.30.0 or above.
 */
 
int main(void)
{
  CURL *curl;
  CURLcode res = CURLE_OK;
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_USERNAME, "2019csb1071@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "assignment5");
    curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/");

         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
     curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "CREATE filesystem");

    // curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "EXAMINE mai");
    res = curl_easy_perform(curl);
 
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* Always cleanup */
    curl_easy_cleanup(curl);
  }
 
  return (int)res;
}