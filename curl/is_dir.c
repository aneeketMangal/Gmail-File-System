/*
    function to check if a given path is valid or not
    returns 0; when path is invalid
    returns 1; when path is valid
*/
int is_dir(char* base, char* relUrl){
    CURL * curl = curl_init();
    chunk_init;
    char url[strlen(base)+256];
    // checking using EXAMINE statement of IMAP protocol
    // EXAMINE returns a list of all the mailboxes in the current folder
    // In case does not exist, it returns an error
    // In case exists, we can confirm its a valid dictionary
    strcpy(url, "EXAMINE ");
    strcat(url, base);
    strcat(url, relUrl);
    printf("tepm: %s\n", url);
    curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/filesystem");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, url);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    chunk_free;
    // free(curl);
    if(res!= CURLE_OK){
        return 0;
    }
    return 1;
}