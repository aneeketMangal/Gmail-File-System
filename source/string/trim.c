/*
    This function trims the leading and trailing whitespaces
    from a string 
*/

char *trim(char *str)
{
    char * end;
    while(isspace((unsigned char)*str)) 
        str++;
    if(*str == 0)  // string is empty
        return str;
    end = str + strlen(str) - 1;
    /*
        while there is a space at the end of the string, move the end pointer to left
        until it reaches the first non-space character
    */
    while(end > str && isspace((unsigned char)*end))
        end--;
    end[1] = '\0';
    return str;
}
