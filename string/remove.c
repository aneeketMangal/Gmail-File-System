/*
    function to remove a given substring from some string
    assumption: substring must exist in the string
*/

char *strrem(char *str, const char *sub) {
    
        char *p = str;
        p = strstr(p, sub);
        memmove(p, p + strlen(sub), strlen(p + strlen(sub)) + 1);
        
    
    return str;
}