void addDir(const char *dir_name)
{
    CURL * curl = curl_init();
    chunk_init;
	char b[DIR_NAME+50] = "CREATE filesystem";
	char a[DIR_NAME+50];
	strcpy(a, dir_name);
	strcat(b, a);
	/*
		using curl to create a corresponding directory on the server, 
	*/
    curl_easy_setopt(curl, CURLOPT_URL, IMAP_BASE_PATH);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, b); 
	curl_easy_perform(curl);
    curl_easy_cleanup(curl);
}