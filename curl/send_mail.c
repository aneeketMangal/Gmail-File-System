char payload_text[5000];

struct upload_status {
  size_t bytes_read;
};

static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_status *upload_ctx = (struct upload_status *)userp;
  const char *data;
  size_t room = size * nmemb;
 
  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }
 
  data = &payload_text[upload_ctx->bytes_read];
 
  if(*data) {
    size_t len = strlen(data);
    if(room < len)
      len = room;
    memcpy(ptr, data, len);
    upload_ctx->bytes_read += len;
 
    return len;
  }
 
  return 0;
}
 
void send_mail(char* path, const char *body){
    printf("Sending mail: %s\n", path);
    CURL *curl = curl_init();
    payload_text[0] = '\0';
    
    struct upload_status upload_ctx = {0};
    curl_easy_setopt(curl,CURLOPT_VERBOSE,1L);
    
    char * fileName = strrchr(path, '/'); // fetching the file name
    fileName++; // skipping the '/'
    char * directory = malloc(sizeof(char)*(strlen(path)-strlen(fileName))); // fetching the directory
    strncpy(directory, path, strlen(path)-strlen(fileName)-1);
    directory[strlen(path)-strlen(fileName)-1] = '\0';

    strcat(payload_text, "From: Aneeket\r\n");
    strcat(payload_text, "Subject: ");
    strcat(payload_text, fileName);
    strcat(payload_text, "\r\n\r\n");
    strcat(payload_text, body);
    strcat(payload_text, "\r\n");

    printf("%s\n", payload_text);

    char url[512];
    strcpy(url, IMAP_BASE_PATH);
    strcat(url, directory);
    strcat(url, "/");
    printf("url: %s\n", url);
    curl_easy_setopt(curl, CURLOPT_URL, url);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
 
    long infilesize = strlen(payload_text);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE, infilesize);
    CURLcode res = curl_easy_perform(curl);
 
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* Always cleanup */
    curl_easy_cleanup(curl);

}