/*
    This function returns the substring str[m:n] of the given string str.
*/

char* substr(char *src, int start, int end)
{
    int len = end-start; //length of the string
    char *dest = (char*)malloc(sizeof(char) * (len+1));

    for (int i = start; i < end; i+=1){
        dest[i-start] = src[i];
    }
    dest[len] = '\0'; //terminating the string
    return dest; // returning the required substring
}

