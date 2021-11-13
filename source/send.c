// #include <stdio.h>
// #include <string.h>
// #include <curl/curl.h>
 
// /*
//  * For an SMTP example using the multi interface please see smtp-multi.c.
//  */
 
// /* The libcurl options want plain addresses, the viewable headers in the mail
//  * can very well get a full name as well.
//  */
// #define FROM_ADDR    "2019csb1071@iitrpr.ac.in"
// // #define TO_ADDR      "2019csb1071@iitrpr.ac.in"
// #define TO_ADDR      "aneeketmangal@gmail.com"
 
// #define FROM_MAIL "Sender Person " FROM_ADDR
// #define TO_MAIL   "A Receiver " TO_ADDR
// #define CC_MAIL   "John CC Smith " CC_ADDR
 
// static const char *payload_text =
//   "Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n"
//   "To: " TO_MAIL "\r\n"
//   "From: " FROM_MAIL "\r\n"
//   "Cc: \r\n"
//   "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
//   "rfcpedant.example.org>\r\n"
//   "Subject: SMTP example message\r\n"
//   "\r\n" /* empty line to divide headers from body, see RFC5322 */
//   "The body of the message starts here.\r\n"
//   "\r\n"
//   "It could be a lot of lines, could be MIME encoded, whatever.\r\n"
//   "Check RFC5322.\r\n";
 
// struct upload_status {
//   size_t bytes_read;
// };
 
// static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
// {
//   struct upload_status *upload_ctx = (struct upload_status *)userp;
//   const char *data;
//   size_t room = size * nmemb;
 
//   if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
//     return 0;
//   }
 
//   data = &payload_text[upload_ctx->bytes_read];
 
//   if(data) {
//     size_t len = strlen(data);
//     if(room < len)
//       len = room;
//     memcpy(ptr, data, len);
//     upload_ctx->bytes_read += len;
 
//     return len;
//   }
 
//   return 0;
// }
 
// int main(void){
//   CURL *curl;
//   CURLcode res = CURLE_OK;
//   struct curl_slist *recipients = NULL;
//   struct upload_status upload_ctx = { 0 };
 
//   curl = curl_easy_init();
//   if(curl) {
//     recipients = curl_slist_append(recipients, TO_ADDR);
//     curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
 
//     /* We are using a callback function to specify the payload (the headers and
//      * body of the message). You could just use the CURLOPT_READDATA option to
//      * specify a FILE pointer to read from. */
    
//     curl_easy_setopt(curl, CURLOPT_USERNAME, "2019csb1071@iitrpr.ac.in");
//      curl_easy_setopt(curl, CURLOPT_PASSWORD, "207kavi4067");
//      curl_easy_setopt(curl, CURLOPT_URL, "smtp.gmail.com:465/");
//      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
//      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
//      curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
//      curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
//     curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
//     curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
//      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
 
//     /* Send the message */
//     res = curl_easy_perform(curl);
 
//     /* Check for errors */
//     if(res != CURLE_OK)
//       fprintf(stderr, "curl_easy_perform() failed: %s\n",
//               curl_easy_strerror(res));
 
//     curl_slist_free_all(recipients);

//     curl_easy_cleanup(curl);
//   }
 
//   return (int)res;
// }


#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

struct url_data {
    size_t size;
    char* data;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
    size_t index = data->size;
    size_t n = (size * nmemb);
    char* tmp;

    data->size += (size * nmemb);

#ifdef DEBUG
    fprintf(stderr, "data at %p size=%ld nmemb=%ld\n", ptr, size, nmemb);
#endif
    tmp = realloc(data->data, data->size + 1); /* +1 for '\0' */

    if(tmp) {
        data->data = tmp;
    } else {
        if(data->data) {
            free(data->data);
        }
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';

    return size * nmemb;
}

char *handle_url() {
    CURL *curl;

    struct url_data data;
    data.size = 0;
    data.data = malloc(4096); /* reasonable size initial buffer */
    if(NULL == data.data) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    data.data[0] = '\0';

    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1 );
    curl_easy_setopt(curl, CURLOPT_USERNAME, "2019csb1071@iitrpr.ac.in");
     curl_easy_setopt(curl, CURLOPT_PASSWORD, "207kavi4067");
     curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:465");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
     printf("meta\n");
    //  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L)s;

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",  
                        curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);

    }
    return data.data;
}

int main(int argc, char* argv[]) {
    char* data;
    data = handle_url();

    if(data) {
        printf("%s\n", data);
        free(data);
    }

    return 0;
}