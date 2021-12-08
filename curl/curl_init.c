/*
    Function to initialise curl
    with defauly settings
*/

CURL* curl_init(){
	CURL * curl;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_USERNAME, USER_NAME);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, PASSWORD);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	return curl; 
}
